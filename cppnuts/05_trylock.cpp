#include <mutex>
#include <thread>
#include <iostream>
#include <chrono>

using namespace std;

int x=0, y=0;
mutex m1, m2;

void doWorkForSeconds(int sec) {
    this_thread::sleep_for(chrono::seconds(sec));
}

void increment(int &xory, mutex &m, const char *desc) {
    for(int i = 0; i < 5; ++i) {
        m.lock();
        ++xory;
        cout << "[I] " << desc << ", " << xory << endl;
        m.unlock();
        doWorkForSeconds(1);
    }
}

void consumeXY() {
    int useCount = 5;
    int xplusy = 0;
    while (1) {
       int lockRes = try_lock(m1, m2);
       if(lockRes == -1) {
           if(x != 0 && y != 0) {
               --useCount;
               xplusy += (x + y);
               x = 0;
               y = 0;
               cout << "[I] xplusy: " << xplusy << endl;
           }
           m1.unlock();
           m2.unlock();
           if(useCount == 0) break;
       }
    }
}

int main() {
    thread t1(increment, ref(x), ref(m1), "X");
    thread t2(increment, ref(y), ref(m2), "Y");
    thread t3(consumeXY);

    t1.join();
    t2.join();
    t3.join();
    return 0;
}
