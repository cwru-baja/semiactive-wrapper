#include <iostream>
#include <vector>
#include <thread>
#include "WheelWrapper.hpp"
#include "SensorPrint.hpp"
#include "ZMQUpdater.hpp"
#include <zmq.hpp>

int main() {

    // make sensor objects
    ZMQSensorData sensors;
    ZMQOutput output;

    // create the 4 wheel controllers
    WheelWrapper fl(1000, "FL_Wheel", "cwrubaja.suspension.fl", sensors, output);
    WheelWrapper fr(1001, "FR_Wheel", "cwrubaja.suspension.fr", sensors, output);
    WheelWrapper bl(1002, "RL_Wheel", "cwrubaja.suspension.bl", sensors, output);
    WheelWrapper br(1003, "RR_Wheel", "cwrubaja.suspension.br", sensors, output);

    SensorPrint sp(sensors);

    ZMQUpdater zu(sensors, output);

    // launch the threads asynchronously
    std::vector<std::thread> threads;

    // lambdas to start the run member function
    threads.emplace_back([&fl](){ fl.run(); });
    threads.emplace_back([&fr](){ fr.run(); });
    threads.emplace_back([&bl](){ bl.run(); });
    threads.emplace_back([&zu](){ zu.run(); });

    threads.emplace_back([&br](){ br.run(); });

    // print sensors
    threads.emplace_back([&sp](){ sp.run(); });

    // join threads (this will block main forever, as intended)
    for (auto& t : threads) {
        if(t.joinable()) t.join();
    }

    return 0;
}