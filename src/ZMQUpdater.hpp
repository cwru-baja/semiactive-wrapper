#include <iostream>
#include <vector>
#include <thread>
#include "WheelWrapper.hpp"
#include "Logger.hpp"
#include <regex>

// brew install zeromq cppzmq
#include <zmq.hpp>
#include "Configs.hpp"

class ZMQUpdater {
private:
    // references to sensor and output data
    ZMQSensorData& sensors;
    ZMQOutput&     output;

    // logger reference
    Logger& logger;

    // zmq objects
    zmq::context_t zmq_context;
    zmq::socket_t  socket_in;
    zmq::socket_t  socket_out;

public:
    ZMQUpdater(std::string zmq_in, std::string zmq_out, ZMQSensorData& sensors_object, ZMQOutput& output_object, Logger& logger_object) 
        : sensors(sensors_object), output(output_object), logger(logger_object){

        zmq_context = zmq::context_t(1); // zmq context
            
        // setup zmq  input socket
        socket_in = zmq::socket_t(zmq_context, zmq::socket_type::sub); // subscriber socket
        socket_in.set(zmq::sockopt::subscribe, "");
        socket_in.connect(zmq_in);

        // setup zmq output socket
        socket_out = zmq::socket_t(zmq_context, zmq::socket_type::pub); // publisher socket
        socket_out.bind(zmq_out);

        // log zmq updater start
        logger.log("ZMQUpdater", "ZMQUpdater initialized. Subscribed to: " + zmq_in + ", publishing to: " + zmq_out);
    }

    // get all unread zmq messages
    std::vector<std::string> getAllUnreadZMQMessages(zmq::socket_t& subSocket) {
        std::vector<std::string> messages;
        zmq::message_t msg;
        while (subSocket.recv(msg, zmq::recv_flags::dontwait)) {
            messages.push_back(msg.to_string());
        }
        return messages;
    }

    // parse subject_id and value from input string
    std::pair<int, double> parseSubjectAndValue(const std::string& s) {
        // Match: {'subject_id': 32, 'value': 624.45}
        std::regex re(R"(\{'subject_id':\s*(\d+),\s*'value':\s*([0-9]+(?:\.[0-9]+)?)\})");
        std::smatch match;

        if (std::regex_search(s, match, re)) {
            int subject_id = std::stoi(match[1]);
            double value   = std::stod(match[2]);
            return {subject_id, value};
        } else {
            throw std::runtime_error("Failed to parse input string");
        }
    }

    // update sensors from ZMQ messages
    void updateSensorsFromZMQMessages() {
        auto messages = getAllUnreadZMQMessages(socket_in);
        if (messages.empty()) return; // no messages to process
        for (const auto& msg : messages) {
            try {
                auto [subject_id, value] = parseSubjectAndValue(msg);
                Sensor& sensor = sensors.getAt(subject_id);
                sensor.value = value;
            } catch (const std::exception& e) {
                continue; // ignore malformed messages, or unknown subject_ids
            }
        }
    }

    
    // publish output setpoints to ZMQ
    void publishOutputToZMQ() {

        // evan whats this format:
        // '{"priority": 2, "type":"cwrubaja.suspension.vcm.Setpoint.0.2", "current": {"ampere": 67.0}, "subject_id": 127}'
        // and evan gets what evan wants
        // so here goes
        auto format = [](int sub_id, double setpoint) {
            return "{\"priority\": 2, \"type\":\"cwrubaja.suspension.vcm.Setpoint.0.2\", \"current\": {\"ampere\": " + std::to_string(setpoint) + "}, \"subject_id\": " + std::to_string(sub_id) + "}";
        };

        // send setpoints for each wheel
        socket_out.send(zmq::message_t(format(CONFIG_FL_SUBJECT_ID, output.FL_setpoint)), zmq::send_flags::dontwait);
        socket_out.send(zmq::message_t(format(CONFIG_FR_SUBJECT_ID, output.FR_setpoint)), zmq::send_flags::dontwait);
        socket_out.send(zmq::message_t(format(CONFIG_BL_SUBJECT_ID, output.BL_setpoint)), zmq::send_flags::dontwait);
        socket_out.send(zmq::message_t(format(CONFIG_BR_SUBJECT_ID, output.BR_setpoint)), zmq::send_flags::dontwait);
    }


    void run() {
        try {
            while (true) { // main loop
                updateSensorsFromZMQMessages();
                publishOutputToZMQ();
                usleep(1000); // update every 1ms
            }
        } catch (const std::exception& e) { // fatal error in zmq updater, we are FRIED chat
            logger.log("ZMQUpdater", "ZMQUpdater encountered a fatal error: " + std::string(e.what()));
            throw std::runtime_error("ZMQUpdater encountered a fatal error.");
        }
    }
};
