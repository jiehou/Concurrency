// Topic: lock() in c++11
// It is used to lock multiple mutex at the same time

// IMPORTANT
// syntax: std::lock(m1, m2, m3, m4)
// 0. all arguments are locked via a sequence of calls to lock(), try_lock, or unique_lock() on each argument
// 1. order of locking is not defined (it will try to lock provided mutex in any order and ensure that there is no deadlock)
// 2. it is a blocking call

#include <iostream>
#include <thread>
#include <mutex>
#include <chrono>
using namespace std;

mutex m1, m2;

void task_a() {
    while(1) {
        lock(m1, m2);
        std::cout << "task a\n";
        this_thread::sleep_for(chrono::microseconds(10));
        m1.unlock();
        m2.unlock();
    }
}

void task_b() {
    while(1) {
        lock(m1, m2);
        std::cout << "task b\n";
        this_thread::sleep_for(chrono::microseconds(10));
        m1.unlock();
        m2.unlock();
    }
}

int main() {
    thread ta(task_a);
    thread tb(task_b);
    ta.join();
    tb.join();
    return 0;
}