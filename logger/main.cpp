#include <iostream>
#include <sstream>
#include "logger.h"

using std::cout;
using std::endl;
using std::stringstream;

void LogMessage(int id, Logger& logger) {
    for(int i = 0; i < 10; ++i) {
        stringstream ss;
        ss << "[I] log entry: " << i << ", from thread: " << id;
        //cout << "[D] id: " << id << ", i: " << i << endl;
        logger.Log(ss.str());
    }
}

int main() {
    Logger logger;
    vector<thread> ts;
    for(int i = 0; i < 10; ++i)
        ts.emplace_back(LogMessage, i, ref(logger));
    // wait for all threads to finish
    for(auto& t : ts) t.join();
    cout << "[I] finish the program\n";
}