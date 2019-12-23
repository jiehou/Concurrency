// mutex m1;
// unique_lock<mutex> lock(m1);

#include <iostream>
#include <thread>
#include <mutex>
#include <string>
using namespace std;

// Notes
// 0. the class unique_lock is a mutex ownership wrapper
// 1. it allows
//  a. can have different locking strategies: defer_lock, try_to_lock, adopt_lock
//  b. time-constrained attempts
//  c. recursive locking
//  d. transfer of lock ownership (move not copy)
//  e. condition variables

// locking strategies
// 1. defer_lock: do not acquire ownership of the mutex
// 2. try_to_lock: try to acquire ownership of the mutex without blocking
// 3. adopt_lock:

mutex m1;
int buffer = 0;

void task(const string & s, int loopFor) {
    unique_lock<mutex> lock(m1, defer_lock); // three strategies: defer_lock, try_to_lock, adpot_lock
    //  @NOTE: can do something here
    lock.lock();
    for(int i = 0; i < loopFor; ++i) {
        buffer++;
        cout << s << ": " << buffer << endl;
    }
    // lock.unlock(); // this is not needed as it will be unlocked in destructor of unique_lock
}

int main() {
    thread t0(task, "t0", 10);
    thread t1(task, "t1", 10);
    t0.join();
    t1.join();
    return 0;
}