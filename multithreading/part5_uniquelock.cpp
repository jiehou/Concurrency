#include <iostream>
#include <fstream>

#include <mutex>
#include <chrono>
#include <thread>
#include <functional>
using namespace std;
/**
* Comparing to lock_guard, unique_lock is flexibler:
* 1) it can lock and unlock many times
* 2) it can not be copied, but it can be moved 
*  e.g. unique_lock<mutex> mlock2 = move(mlock1);
*/

/**
* call_once(once_flag& flag, Callable&& f, Args&&... args)
*/
class LogFile{
    ofstream fout_;
    mutex mu_;
    once_flag open_flag_;
public:
    LogFile() {}
    virtual ~LogFile() {
        if(fout_.is_open()) fout_.close();
    }

    void sharedPrint(const string& msg, int number) {
        if(!fout_.is_open()) {
            call_once(open_flag_, [&]() { fout_.open("log.txt"); });
            // NOTE: [=] captures all variables by value
            // [&] captures all variables by reference 
        }
        unique_lock<mutex> mlock(mu_, defer_lock);
        mlock.lock();
        fout_ << msg << ": " << number << endl;
        mlock.unlock();
        this_thread::sleep_for(chrono::seconds(2));
    } // sharedPrint
    
    void printFile() {
        ifstream fin("log.txt");
        string line;
        if(fin.is_open()) {
            while(getline(fin, line)) {
                cout << line << endl;
            }
            fin.close();
        }
    }
};

void writeToLog(LogFile& logger, string name) {
    for(int i = 0; i < 10; ++i) {
        logger.sharedPrint(name, i);
    }
}

int main() {
    cout << "[I] unique_lock\n";
    LogFile logger;
    thread t1(writeToLog, ref(logger), "t1");
    thread t2(writeToLog, ref(logger), "t2");
    t1.join();
    t2.join();
    logger.printFile();
}
