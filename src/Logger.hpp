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

/*
==============================================================================
Logger
==============================================================================

Purpose:
- Provide thread-safe logging utilities for the rest of the control runtime.
- Support both file logging and console logging at the same time.

Design notes:
- Every logging call acquires an internal mutex so messages emitted from
  multiple wheel threads do not interleave within a single line.
- Timestamps are relative to process start and shown in seconds with 2 decimals.
- File names are generated from local wall-clock date/time.

Threading notes:
- The logger is shared by all worker threads (wheel threads + ZMQ thread).
- Methods are intentionally lightweight and string-oriented.
*/
class Logger {
private:
    // Destination stream for file logging mode.
    std::ofstream outFile;

    // Protects all write operations so log lines stay coherent under concurrency.
    std::mutex log_mutex;

    // Runtime flags determining which sinks are active.
    bool file_log;
    bool print_log;

    // Build a filename-safe date-time stamp for log file naming.
    // Format: YYYY-MM-DD_HH:MM:SS
    std::string current_datetime() {

        std::time_t t = std::chrono::system_clock::to_time_t(
            std::chrono::system_clock::now()
        );

        std::tm tm{};
        // Convert to local time in a thread-safe way on POSIX platforms.
        // For Windows builds, localtime_s(&tm, &t) is the equivalent.
        localtime_r(&t, &tm);

        std::ostringstream oss;
        oss << std::put_time(&tm, "%Y-%m-%d_%H:%M:%S");
        return oss.str();
    }

    // Get elapsed seconds since first invocation of this helper.
    // The static start time is initialized exactly once.
    std::string getTimeSinceStart() {
        static auto start_time = std::chrono::high_resolution_clock::now();
        auto current_time = std::chrono::high_resolution_clock::now();
        std::chrono::duration<double> elapsed = current_time - start_time;
        std::ostringstream oss;
        // Keep timestamps compact and human-readable.
        oss << std::fixed << std::setprecision(2) << elapsed.count();
        return oss.str(); 
    }
public:
    // Constructor
    // - do_file_log: create and write to a log file under logs/
    // - do_print_log: mirror logs to stdout
    //
    // Side effect:
    // - Initializes the relative timer baseline by calling getTimeSinceStart().
    Logger(bool do_file_log, bool do_print_log) : file_log(do_file_log), print_log(do_print_log) {
        if (file_log) {
            // Open a timestamped file for this run.
            outFile = std::ofstream("logs/" + current_datetime() + ".log");
            outFile << "Log started at " << current_datetime() << std::endl;
        }
        // Force initialization of the relative timer baseline now.
        getTimeSinceStart();
    };

    // Log when a wheel control loop transitions into active execution.
    void logStart(std::string name) {
        std::lock_guard<std::mutex> lock(log_mutex);
        std::string str = "( " + getTimeSinceStart() + " ) [" + name + "] Starting Control Loop...";
        if (file_log) outFile << str << std::endl;
        if (print_log) std::cout << str << std::endl;
    }

    // Log a watchdog/frame timeout in the algorithm update path.
    void logTimeout(std::string name) {
        std::lock_guard<std::mutex> lock(log_mutex);
        std::string str = "( " + getTimeSinceStart() + " ) [" + name + "] Work Function Timeout!";
        if (file_log) outFile << str << std::endl;
        if (print_log) std::cout << str << std::endl;
    }

    // Log an internal frame error that does not match a timeout condition.
    void logUnknownError(std::string name) {
        std::lock_guard<std::mutex> lock(log_mutex);
        std::string str = "( " + getTimeSinceStart() + " ) [" + name + "] Unknown Frame Error!";
        if (file_log) outFile << str << std::endl;
        if (print_log) std::cout << str << std::endl;
    }

    // Log transition to fail-safe mode after repeated critical events.
    void logCriticalFailure(std::string name) {
        std::lock_guard<std::mutex> lock(log_mutex);
        std::string str = "( " + getTimeSinceStart() + " ) [" + name + "] CRITICAL FAILURE! REVERTING TO EMERGENCY MODE SETPOINT!";
        if (file_log) outFile << str << std::endl;
        if (print_log) std::cout << str << std::endl;
    }

    // Emit periodic performance stats for one wheel thread.
    // Parameters:
    // - avg_algo_time: average algorithm-only runtime (ms)
    // - avg_frame_time: average full frame runtime including synchronization (ms)
    // - frame_offset_ms: dynamic offset used by WheelWrapper timing calibration
    // - update_dumps: count of update threads detached due to timeout
    void logStats(const std::string& name, double avg_algo_time, double avg_frame_time, 
                  double frame_offset_ms, int update_dumps) {
        std::lock_guard<std::mutex> lock(log_mutex);
        std::ostringstream oss;

        // Keep this parameter for API completeness with caller statistics.
        // It is currently not printed in the string below.
        (void)frame_offset_ms;

        oss << "( " << getTimeSinceStart() << " ) [" << name << "] ";
        oss << "{ Avg Algo: " << std::fixed << std::setprecision(2) << avg_algo_time << " ms,";
        // Convert average period in milliseconds to frequency in hertz:
        // Hz = 1000 / period_ms
        oss << " Avg Frame Duration: " << std::fixed << std::setprecision(2) << avg_frame_time << " ms (" << std::fixed << std::setprecision(2) << 1000/avg_frame_time << " Hz),";
        oss << " Update Dumps: " << update_dumps << " }";

        std::string str = oss.str();
        if (file_log) outFile << str << std::endl;
        if (print_log) std::cout << str << std::endl;
    }

    // General-purpose log line helper for arbitrary status messages.
    void log(const std::string& name ,const std::string& message) {
        std::lock_guard<std::mutex> lock(log_mutex);
        std::string str = "( " + getTimeSinceStart() + " ) [" + name + "] " + message;
        if (file_log) outFile << str << std::endl;
        if (print_log) std::cout << str << std::endl;
    }

};

#endif
