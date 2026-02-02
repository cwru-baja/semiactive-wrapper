#include <iostream>
#include <vector>
#include <thread>
#include <zmq.hpp>

#include "WheelWrapper.hpp"
#include "ZMQUpdater.hpp"
#include "Utils.hpp"
#include "Logger.hpp"
#include "Configs.hpp"

int main() {

    // shared algorithm memory
    SharedAlgorithmMemory shared_memory;

    // make sensor objects
    ZMQSensorData sensors;
    ZMQOutput     output;

    // start the logger
    Logger logger = Logger(true, true); // file logging (to ./log), print (std:cout) logging

    // start the ZMQ updater
    ZMQUpdater zu(CONFIG_ZMQ_IN, CONFIG_ZMQ_OUT, sensors, output, logger);

    // create the 4 wheel controllers
    WheelWrapper fl(CONFIG_FL_SUBJECT_ID, CONFIG_FL_UNIQUE_NAME, CONFIG_UPDATE_HZ, CONFIG_MAX_UPDATE_DUMPS, sensors, output, shared_memory, logger);
    WheelWrapper fr(CONFIG_FR_SUBJECT_ID, CONFIG_FR_UNIQUE_NAME, CONFIG_UPDATE_HZ, CONFIG_MAX_UPDATE_DUMPS, sensors, output, shared_memory, logger);
    WheelWrapper bl(CONFIG_BL_SUBJECT_ID, CONFIG_BL_UNIQUE_NAME, CONFIG_UPDATE_HZ, CONFIG_MAX_UPDATE_DUMPS, sensors, output, shared_memory, logger);
    WheelWrapper br(CONFIG_BR_SUBJECT_ID, CONFIG_BR_UNIQUE_NAME, CONFIG_UPDATE_HZ, CONFIG_MAX_UPDATE_DUMPS, sensors, output, shared_memory, logger);
    

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
        usleep(1000000); // sleep to prevent busy wait
    }

    // if we are here, we are kinda cooked ngl 
    // this is really just to satisfy the compiler
    logger.log("MAIN", "Main thread exiting (this should never happen). All wheels have stopped. Program terminated.");

    return 0;
}