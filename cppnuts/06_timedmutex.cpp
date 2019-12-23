/**
 * 1) timed_mutex is blocked till timeout_time or the lock is acquired and returns true if success
 * otherwise false
 * 2) member function
 * a. lock
 * b. try_lock 
 * c. try_lock_for
 * d. try_lock_until
 * e. unlock 
*/
#include <iostream>
#include <thread>
#include <mutex>
#include <chrono>

using namespace std;

int myInt = 0;
timed_mutex m;

void increment(int i) {
    if(m.try_lock_for(chrono::seconds(5))) {
        ++myInt;
        this_thread::sleep_for(chrono::seconds(2));
        cout << "[I] thread " << i << " entered.\n";
        m.unlock();
    }
    else {
        cout << "[I] thread " << i << " could not enter.\n";
    }
}

int main() {
    thread t1(increment, 1);
    thread t2(increment, 2);
    t1.join();
    t2.join();

    cout << myInt << endl;
    return 0;
}
