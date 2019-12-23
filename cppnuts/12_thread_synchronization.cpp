// topic: thread or process synchronization
// we will use thread examples to explain this topic

// points:
// 0. thread or process synchronize to access critical section
// 1. critical section is one or collection of program statements which should be executed by only one thread or process
//  at a time

#include <iostream>
#include <thread>
#include <mutex>

using namespace std;

long long bankBalance = 0;
mutex m1;

void addMoney(long long val) {
    if(val <= 0) return;
    unique_lock<mutex> ul(m1);
    bankBalance += val;
}

int main() {
    thread t1(addMoney, 100);
    thread t2(addMoney, 200);
    t1.join();
    t2.join();
    cout << "[I] final bankBalance: " << bankBalance << endl;
    return 0;
}