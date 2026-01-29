#include <iostream>
#include <sys/time.h>
#include <unistd.h>
#include <string>
#include <regex>

#include "Utils.hpp"

#include <zmq.hpp>

class SensorPrint {
private:

    ZMQSensorData& sensors;

public:
    SensorPrint(ZMQSensorData& sensors_object) : sensors(sensors_object) {}

    void run() {
        try {
            while (true) {

                std::cout << "----------------------" << std::endl;

                for (size_t i = 1; i <= sensors.sensorMapping.size(); i++)
                {
                    std::cout << "Sensor [" << i << "] '" << sensors.getAt(i).name << "' Reading: " << sensors.getAt(i).value << std::endl;
                }

                std::cout << "----------------------" << std::endl;

                usleep(1000000);
            }
        } catch (const std::exception& e) {
            ; // ignore
        }
    }
};