#include <iostream>
#include <string>
#include <mutex>
#include <thread>
#include <fstream>
#include <memory>
using namespace std;

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