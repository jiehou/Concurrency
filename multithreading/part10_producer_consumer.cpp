#include <iostream>
#include <queue>
#include <thread>
#include <chrono>
#include <mutex>
#include <condition_variable>
using namespace std;
using namespace std::chrono;

bool stopFlag = false;
queue<int> buff;
mutex mtx;
int gNum = 0;
condition_variable mcond;

void Producer() {
    while(!stopFlag) {
        unique_lock<mutex> mlock(mtx);
        buff.push(gNum);
        cout << "[I] producer id: " << this_thread::get_id() << ", " << gNum << endl;
        gNum++;
        mlock.unlock();
        mcond.notify_all();
        this_thread::sleep_for(chrono::seconds(2));
    }
}

void Consumer() {
    while(!stopFlag) {
        unique_lock<mutex> mlock(mtx);
        mcond.wait(mlock); // []() { return !stopFlag && !buff.empty(); } 
        if(stopFlag) return;
        if(!buff.empty()) {
            int res = buff.front();
            cout << "[I] consumer id: " << this_thread::get_id() << ", " << res << endl;
            if(res > 50) {
                cout << "[I] will stop\n";
                stopFlag = true;
            }
            buff.pop();
        }
    }
}

int main() {
    thread t1(Producer);
    thread t2(Consumer);
    t1.join();
    t2.join();
    return 0;
}