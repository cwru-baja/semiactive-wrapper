#include <iostream>
#include <sys/time.h>
#include <unistd.h>
#include <string>
#include <regex>

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
                    std::cout << msgs.size() << "----------------------" << std::endl;
                    for (const auto& msg : msgs) {
                        int subject_id = 0;
                        int value = 0;

                        std::regex re(R"(\{'subject_id':\s*(\d+),\s*'value':\s*(\d+)\})");
                        std::smatch match;

                        if (std::regex_search(msg, match, re)) {
                            subject_id = std::stoi(match[1]);
                            value      = std::stoi(match[2]);

                            std::cout << "subject_id = " << subject_id << "\n";
                            std::cout << "value      = " << value << "\n";
                        } else {
                            std::cout << "Failed to parse string\n";
                        }
                    }
                    std::cout << msgs.size() << "----------------------" << std::endl;
                }
                usleep(100);
            }
        } catch (const std::exception& e) {
            printf(e.what());
        }
    }
};