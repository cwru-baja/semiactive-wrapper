#include <iostream>
#include <vector>
#include <thread>
#include "WheelWrapper.hpp"
#include "SensorPrint.hpp"
#include <zmq.hpp>

int main() {

    zmq::context_t zmq_ctx(1);

    // create the 4 wheel controllers
    WheelWrapper fl(1000, "FL_Wheel", "cwrubaja.suspension.fl", zmq_ctx, true);
    WheelWrapper fr(1001, "FR_Wheel", "cwrubaja.suspension.fr", zmq_ctx);
    WheelWrapper bl(1002, "RL_Wheel", "cwrubaja.suspension.bl", zmq_ctx);
    WheelWrapper br(1003, "RR_Wheel", "cwrubaja.suspension.br", zmq_ctx);

    SensorPrint sp(zmq_ctx);

    // launch the threads asynchronously
    std::vector<std::thread> threads;

    // lambdas to start the run member function
    threads.emplace_back([&fl](){ fl.run(); });
    threads.emplace_back([&fr](){ fr.run(); });
    threads.emplace_back([&bl](){ bl.run(); });
    threads.emplace_back([&br](){ br.run(); });

    // print sensors
    threads.emplace_back([&sp](){ sp.run(); });

    // join threads (this will block main forever, as intended)
    for (auto& t : threads) {
        if(t.joinable()) t.join();
    }

    return 0;
}