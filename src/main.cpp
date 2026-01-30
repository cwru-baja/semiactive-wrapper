#include <iostream>
#include <vector>
#include <thread>
#include <zmq.hpp>

#include "WheelWrapper.hpp"
#include "ZMQUpdater.hpp"
#include "Utils.hpp"
#include "Logger.hpp"

int main() {

    // configs for later

    const auto FL_SUBJECT_ID = 1001;
    const auto FL_UNIQUE_NAME = "FL_Wheel";

    const auto FR_SUBJECT_ID = 1002;
    const auto FR_UNIQUE_NAME = "FR_Wheel";

    const auto BL_SUBJECT_ID = 1003;
    const auto BL_UNIQUE_NAME = "BL_Wheel";

    const auto BR_SUBJECT_ID = 1004;
    const auto BR_UNIQUE_NAME = "BR_Wheel";

    const int UPDATE_HZ = 100;
    const int MAX_UPDATE_DUMPS = 10;

    const std::string ZMQ_IN = "ipc:///tmp/cyphal_out"; // zmq inproc for testing
    const std::string ZMQ_OUT = "ipc:///tmp/wrapper_out"; // zmq inproc for testing




    // shared algorithm memory
    SharedAlgorithmMemory shared_memory;

    // make sensor objects
    ZMQSensorData sensors;
    ZMQOutput output;

    // start the logger
    Logger logger = Logger(true, true); // file logging (to ./log), print (std:cout) logging

    // start the ZMQ updater
    ZMQUpdater zu(ZMQ_IN, ZMQ_OUT, sensors, output, logger);

    // create the 4 wheel controllers
    WheelWrapper fl(FL_SUBJECT_ID, FL_UNIQUE_NAME, UPDATE_HZ, MAX_UPDATE_DUMPS, sensors, output, shared_memory, logger);
    WheelWrapper fr(FR_SUBJECT_ID, FR_UNIQUE_NAME, UPDATE_HZ, MAX_UPDATE_DUMPS, sensors, output, shared_memory, logger);
    WheelWrapper bl(BL_SUBJECT_ID, BL_UNIQUE_NAME, UPDATE_HZ, MAX_UPDATE_DUMPS, sensors, output, shared_memory, logger);
    WheelWrapper br(BR_SUBJECT_ID, BR_UNIQUE_NAME, UPDATE_HZ, MAX_UPDATE_DUMPS, sensors, output, shared_memory, logger);
    

    // launch the threads asynchronously
    std::vector<std::thread> threads;
    threads.emplace_back([&fl](){ fl.run(); }); // lambdas to start the run member function
    threads.emplace_back([&fr](){ fr.run(); });
    threads.emplace_back([&bl](){ bl.run(); });
    threads.emplace_back([&br](){ br.run(); });
    threads.emplace_back([&zu](){ zu.run(); });

    // join threads (this will block main forever, as intended)
    for (auto& t : threads) {
        if(t.joinable()) t.join();
        usleep(10000); // sleep to prevent busy wait
    }

    // if we are here, we are kinda cooked ngl 
    // this is really just to satisfy the compiler
    logger.log("MAIN", "Main thread exiting (this should never happen). All wheels have stopped. Program terminated.");

    return 0;
}