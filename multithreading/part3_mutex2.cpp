#include <iostream>
#include <string>
#include <mutex>
#include <thread>
#include <fstream>
#include <memory>
using namespace std;

/* Note:
* 0) mutex, lock_guard
* 1) use mutex to synchronize data cess
* 2) never leak a a handle of data to outside
* e.g. never retrun fout_ to the outside world e.g. ofstream& GetStream() { return fout_; }
* e.g. never pass fout_ as an argument to 
    user provided function void ProcessFile(void fun(ofstream&)) { fun(fout_); }
* 3) desing interface appropriately e.g. a thread-safe stack
*/

class LogFile {
public:
    LogFile(const string& fname) {
        fout_.open(fname);
    }
    virtual ~LogFile() { if(fout_.is_open()) fout_.close(); }
    void SharedPrint(const string& msg, int num) {
        lock_guard<mutex> lg(mu_);
        if(fout_.is_open())
            fout_ << msg << ": " << num << endl;
    }
private:
    mutex mu_;
    ofstream fout_;
};

void Func1(LogFile& log) {
    for(int i = 0; i < 100; ++i) {
        log.SharedPrint("[I] From Func1:", i);
    }
}

int main() {
    LogFile mlog("log1.txt");
    thread t(Func1, ref(mlog));
    for(int i = 0; i < 100; ++i) {
        mlog.SharedPrint("From main: ", i);
    }
    t.join();
}