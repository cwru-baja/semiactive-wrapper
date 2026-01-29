#include <iostream>
#include <vector>
#include <thread>
#include "WheelWrapper.hpp"
#include <regex>

// brew install zeromq cppzmq
#include <zmq.hpp>

class ZMQUpdater {
private:
    // references to sensor and output data
    ZMQSensorData& sensors;
    ZMQOutput& output;

    // zmq objects
    zmq::context_t zmq_context;
    zmq::socket_t socket_in;
    zmq::socket_t socket_out;
public:
    ZMQUpdater(ZMQSensorData& sensors_object, ZMQOutput& output_object) 
        : sensors(sensors_object), output(output_object) {

        zmq_context = zmq::context_t(1);
            
        socket_in = zmq::socket_t(zmq_context, zmq::socket_type::sub);
        socket_in.set(zmq::sockopt::subscribe, "");
        socket_in.connect("ipc:///tmp/cyphal_out"); // zmq inproc for testing

        socket_out = zmq::socket_t(zmq_context, zmq::socket_type::pub);
        socket_out.bind("ipc:///tmp/wrapper_out"); // zmq inproc for
    }

    std::vector<std::string> getAllUnreadZMQMessages(zmq::socket_t& subSocket) {
        std::vector<std::string> messages;
        zmq::message_t msg;
        while (subSocket.recv(msg, zmq::recv_flags::dontwait)) {
            messages.push_back(msg.to_string());
        }
        return messages;
    }

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
        for (const auto& msg : messages) {
            try {
                auto [subject_id, value] = parseSubjectAndValue(msg);
                Sensor& sensor = sensors.getAt(subject_id);
                sensor.value = static_cast<float>(value);
            } catch (const std::exception& e) {
                ; // ignore malformed messages
            }
        }
    }

    void publishOutputToZMQ() {
        // Not implemented yet
        std::string str = "dsflmsf";
        socket_out.send(zmq::message_t(str), zmq::send_flags::dontwait);
    }


    void run() {
        try {
            while (true) {
                updateSensorsFromZMQMessages();
                publishOutputToZMQ();
                usleep(1000); // update every 1ms
            }
        } catch (const std::exception& e) {
            throw std::runtime_error("ZMQUpdater encountered a fatal error."); // we are FRIED chat
        }
    }
};
