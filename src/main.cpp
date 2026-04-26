#include <iostream>
#include <vector>
#include <thread>
#include <unistd.h>

#include "WheelWrapper.hpp"
#include "ZMQUpdater.hpp"
#include "Utils.hpp"
#include "Logger.hpp"
#include "Configs.hpp"

/*
==============================================================================
main.cpp
==============================================================================

Purpose:
- Entry point for the semi-active wrapper runtime.
- Wires together shared state, logging, ZMQ transport, and wheel controllers.

Startup sequence (high level):
1) Create shared data objects used by all worker threads.
2) Create logger used by every subsystem.
3) Create ZMQ updater that bridges inbound/outbound message traffic.
4) Create four wheel control wrappers (one per corner).
5) Launch one thread per wheel plus one thread for the ZMQ updater.
6) Join all threads (expected to block forever during normal operation).

Threading model:
- `main` is primarily a bootstrapper.
- After launching workers, `main` blocks in join() calls so process lifetime is
  tied to worker lifetimes.
*/
int main() {

    // ---------------------------------------------------------------------
    // Shared algorithm memory
    // ---------------------------------------------------------------------
    // One cross-wheel memory object passed into every wheel algorithm call.
    // Use this for coordinated state across wheel controllers.
    SharedAlgorithmMemory shared_memory;

    // ---------------------------------------------------------------------
    // Shared sensor and output containers
    // ---------------------------------------------------------------------
    // `sensors` is updated by the ZMQ updater thread and read by wheel threads.
    // `output` is written by wheel threads and published by the ZMQ updater.
    ZMQSensorData sensors;
    ZMQOutput     output;

    // ---------------------------------------------------------------------
    // Logger initialization
    // ---------------------------------------------------------------------
    // Enable both file and console logging:
    // - file logs go to logs/<timestamp>.log
    // - console logs go to stdout
    Logger logger = Logger(true, true);

    // ---------------------------------------------------------------------
    // ZMQ updater initialization
    // ---------------------------------------------------------------------
    // This object owns ZMQ sockets and will be run in its own thread.
    // It continuously:
    // - reads inbound sensor messages into `sensors`
    // - publishes outbound setpoints from `output`
    ZMQUpdater zu(CONFIG_ZMQ_IN, CONFIG_ZMQ_OUT, sensors, output, logger, false);

    // ---------------------------------------------------------------------
    // Wheel controller construction
    // ---------------------------------------------------------------------
    // Each WheelWrapper instance controls exactly one wheel subject id.
    // All four wrappers share:
    // - the same sensors/output objects
    // - the same shared algorithm memory object
    // - the same logger
    WheelWrapper fl(CONFIG_FL_SUBJECT_ID, CONFIG_FL_UNIQUE_NAME, CONFIG_UPDATE_HZ, CONFIG_MAX_UPDATE_DUMPS, sensors, output, shared_memory, logger);
    WheelWrapper fr(CONFIG_FR_SUBJECT_ID, CONFIG_FR_UNIQUE_NAME, CONFIG_UPDATE_HZ, CONFIG_MAX_UPDATE_DUMPS, sensors, output, shared_memory, logger);
    WheelWrapper bl(CONFIG_BL_SUBJECT_ID, CONFIG_BL_UNIQUE_NAME, CONFIG_UPDATE_HZ, CONFIG_MAX_UPDATE_DUMPS, sensors, output, shared_memory, logger);
    WheelWrapper br(CONFIG_BR_SUBJECT_ID, CONFIG_BR_UNIQUE_NAME, CONFIG_UPDATE_HZ, CONFIG_MAX_UPDATE_DUMPS, sensors, output, shared_memory, logger);
    

    // ---------------------------------------------------------------------
    // Thread launch
    // ---------------------------------------------------------------------
    // Start one thread per wheel wrapper plus one thread for the ZMQ updater.
    // Lambdas capture references so each thread executes run() on the existing
    // objects defined above.
    std::vector<std::thread> threads;
    threads.emplace_back([&fl](){ fl.run(); });
    threads.emplace_back([&fr](){ fr.run(); });
    threads.emplace_back([&bl](){ bl.run(); });
    threads.emplace_back([&br](){ br.run(); });
    threads.emplace_back([&zu](){ zu.run(); });

    // ---------------------------------------------------------------------
    // Thread join phase
    // ---------------------------------------------------------------------
    // Join all worker threads.
    // In intended operation, workers run forever, so the first successful join()
    // call blocks indefinitely and `main` does not continue past this loop.
    for (auto& t : threads) {
        if(t.joinable()) t.join();

        // This sleep is effectively never reached during normal operation,
        // because join() above is expected to block forever.
        usleep(1000000);
    }

    // ---------------------------------------------------------------------
    // Unexpected shutdown path
    // ---------------------------------------------------------------------
    // Reaching this point means every worker thread exited.
    // That is not expected in the normal runtime model.
    logger.log("MAIN", "Main thread exiting (this should never happen). All wheels have stopped. Program terminated.");

    // Return success to the host OS.
    return 0;
}