#ifndef LOGGER_HPP
#define LOGGER_HPP

#include <fstream>
#include <iostream>
#include <chrono>
#include <ctime>
#include <iomanip>
#include <string>
#include <mutex>
#include <sstream>
#include <cmath>

// Simple Logger class for logging events to file and console
class Logger {
private:
    std::ofstream outFile; // log file stream
    std::mutex log_mutex; // mutex for thread-safe logging

    bool file_log; // do file logging
    bool print_log; // do print logging

    // get current date-time string for log file naming
    std::string current_datetime() {

        std::time_t t = std::chrono::system_clock::to_time_t(
            std::chrono::system_clock::now()
        );

        std::tm tm{};
        localtime_r(&t, &tm);   // Windows: localtime_s(&tm, &t)

        std::ostringstream oss;
        oss << std::put_time(&tm, "%Y-%m-%d_%H:%M:%S");
        return oss.str();
    }

    // get seconds since program start
    std::string getTimeSinceStart() {
        static auto start_time = std::chrono::high_resolution_clock::now();
        auto current_time = std::chrono::high_resolution_clock::now();
        std::chrono::duration<double> elapsed = current_time - start_time;
        // round to 2 decimal places
        std::ostringstream oss;
        oss << std::fixed << std::setprecision(2) << elapsed.count();
        return oss.str(); 
    }
public:
    Logger(bool do_file_log, bool do_print_log) : file_log(do_file_log), print_log(do_print_log) {
        outFile = std::ofstream("logs/" + current_datetime() + ".log"); // open log file
        outFile << "Log started at " << current_datetime() << std::endl;
        getTimeSinceStart(); // initialize start time
    };

    void logStart(std::string name) {
        std::lock_guard<std::mutex> lock(log_mutex);
        std::string str = "( " + getTimeSinceStart() + " ) [" + name + "] Starting Control Loop...";
        if (file_log) outFile << str << std::endl;
        if (print_log) std::cout << str << std::endl;
    }

    void logTimeout(std::string name) {
        std::lock_guard<std::mutex> lock(log_mutex);
        std::string str = "( " + getTimeSinceStart() + " ) [" + name + "] Work Function Timeout!";
        if (file_log) outFile << str << std::endl;
        if (print_log) std::cout << str << std::endl;
    }

    void logUnknownError(std::string name) {
        std::lock_guard<std::mutex> lock(log_mutex);
        std::string str = "( " + getTimeSinceStart() + " ) [" + name + "] Unknown Frame Error!";
        if (file_log) outFile << str << std::endl;
        if (print_log) std::cout << str << std::endl;
    }

    void logCriticalFailure(std::string name) {
        std::lock_guard<std::mutex> lock(log_mutex);
        std::string str = "( " + getTimeSinceStart() + " ) [" + name + "] CRITICAL FAILURE! REVERTING TO EMERGENCY MODE SETPOINT!";
        if (file_log) outFile << str << std::endl;
        if (print_log) std::cout << str << std::endl;
    }

    void logStats(const std::string& name, double avg_algo_time, double avg_frame_time, 
                  double frame_offset_ms, int update_dumps) {
        std::lock_guard<std::mutex> lock(log_mutex);
        std::ostringstream oss;

        oss << "( " << getTimeSinceStart() << " ) [" << name << "] ";
        oss << "{ Avg Algo: " << std::fixed << std::setprecision(2) << avg_algo_time << " ms, ";
        oss << " Avg Frame: " << std::fixed << std::setprecision(2) << avg_frame_time << " ms, ";
        oss << " Update Dumps: " << update_dumps << " }";

        std::string str = oss.str();
        if (file_log) outFile << str << std::endl;
        if (print_log) std::cout << str << std::endl;
    }

    // general log function
    void log(const std::string& message) {
        std::lock_guard<std::mutex> lock(log_mutex);
        if (file_log) outFile << message << std::endl;
        if (print_log) std::cout << message << std::endl;
    }

};

#endif
