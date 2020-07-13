#include "logger.h"
#include <fstream>
#include <iostream>

using std::ofstream;

Logger::Logger() : stop_(false) {
    mthread_ = thread{&Logger::ProcessEntries_, this}; // start background thread
}

Logger::~Logger() {
    {
        unique_lock<mutex> mlock(mtx_);
        stop_ = true;
        cv_.notify_all();
    }
    if(mthread_.joinable()) {
        cout << "[D] joinable\n";
        mthread_.join();
    }
}

void Logger::Log(const string& entry) {
    unique_lock<mutex> lock(mtx_); // lock mutex and add entry to the queue
    q_.push(entry);
    cv_.notify_all(); // notify condition variable to wake up thread
}

void Logger::ProcessEntries_() {
    ofstream ofs("log.txt");
    if(ofs.fail()) {
        cerr << "Failed to open logfile.\n";
        return;
    }
    // start processing loop
    unique_lock<mutex> mlock(mtx_);
    while(true) {
        if(!stop_)
            cv_.wait(mlock);
        // @TODO: condition variable notified, something might be in the queue
        mlock.unlock();
        while(true) {
            mlock.lock();
            if(q_.empty()) {
                mlock.unlock();
                break;
            }
            else {
                ofs << q_.front() << endl;
                q_.pop();
                mlock.unlock();
            }
        } // inner while
        if(stop_) {
            cout << "[I] break from ProcessEntries_()\n";
            break;
        }
    }// outer while
}