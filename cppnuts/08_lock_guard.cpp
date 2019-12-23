// lock_guard<mutex> lock(m1)
#include <iostream>
#include <thread>
#include <mutex>
#include <string>
using namespace std;

// notes
// 0. it is a light wrapper for owning mutex on scoped basis
// 1. it acquires mutex lock at the moment you create the object of lock_guard
// 2. it automatically releases the lock when lock object goes out of scope
// 3. you cannot explicitly unlock the lock_guard

mutex m1;
int buffer = 0;

void task(const string &s, int loopFor) {
    lock_guard<mutex> lock(m1);
    for(int i = 0; i < loopFor; ++i) {
        buffer++;
        cout << s << ": " << buffer << endl;
    }
}

int main() {
    thread t0(task, "t0", 10);
    thread t1(task, "t1", 10);
    t0.join();
    t1.join();
    return 0;
}