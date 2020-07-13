#ifndef LOGGER_H_
#define LOGGER_H_

#include <queue>
#include <string>
#include <thread>
#include <mutex>
#include <condition_variable>

using namespace std;

class Logger {
public:
    Logger(); // start a background thread writing log entries to a file
    Logger(const Logger& src) = delete; // prevent copy construction and assignment
    Logger& operator=(const Logger& rhs) = delete;
    virtual ~Logger(); // @IMPROVED gracefully shut down background thread
    void Log(const string& entry);
private:
    void ProcessEntries_();
    mutex mtx_;
    condition_variable cv_; 
    queue<string> q_;
    thread mthread_;
    bool stop_;
};
#endif