#include <iostream>
#include <sys/time.h>
#include <unistd.h>

#include <zmq.hpp>

class SensorPrint {
private:
    zmq::context_t zmq_context;
    zmq::socket_t socket_in;

    std::vector<std::string> getAllUnreadZMQMessages(zmq::socket_t& subSocket) {
        std::vector<std::string> messages;
        zmq::message_t msg;
        while (subSocket.recv(msg, zmq::recv_flags::dontwait)) {
            messages.push_back(msg.to_string());
        }
        return messages;
    }

public:
    SensorPrint(zmq::context_t& zmq_cxt) {
        this->zmq_context.swap(zmq_cxt);
        this->socket_in = zmq::socket_t(zmq_context, zmq::socket_type::sub);
        socket_in.set(zmq::sockopt::subscribe, "");
        socket_in.connect("ipc:///tmp/cyphal_out"); // zmq inproc for testing
    }
    void run() {
        try {
            while (true) {
                // get messages from zmq
                auto msgs = getAllUnreadZMQMessages(socket_in);
                if (!msgs.empty()) {
                    for (const auto& msg : msgs) {
                        std::cout << msg << std::endl;
                    }
                    std::cout << msgs.size() << " messages." << std::endl;
                }
                usleep(100);
            }
        } catch (const std::exception& e) {
            printf(e.what());
        }
    }
};