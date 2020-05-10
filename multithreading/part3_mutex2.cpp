#include <iostream>
#include <string>
#include <mutex>
#include <thread>
#include <fstream>

using namespace std;

class LogFile {
public:
    LogFile(const string& fname) {
        fout_.open()
    }
private:
    mutex mu_;
    ofstream fout_;
};