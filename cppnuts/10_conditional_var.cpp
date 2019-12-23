// Topic: condition variable in c++
// Purpose of condition variable:
//  a. notify other thread
//  b. waiting for some conditions

// 0. condition variable allows running threads to wait on some conditions and
//  once those conditions are met, the waiting thread is notified using:
//  a. notify_one()
//  b. notify_all()
// 1. mutex is needed to use condition variable
// 2. if some thread wants to wait on some condition, it has to do these things:
//  a. acquire the mutex lock: std::unique_lock<std::mutex>(m1)
//  b. execute wait, wait_for or wait_unitil. the wait operations automatically release teh mutex
//      and suspend the execution of the thread
//  c. when the condition variable is notified, the thread is awakened. and the mutex is automatically
//      required. the thread should then check the condition and resume waiting if the wake up is not true

// NOTE
// 0. condition variables are used to synchronize two or more threads
// 1. best use case of condition variable is producer/consumer problem
#include <iostream>
#include <thread>
#include <mutex>
#include <condition_variable>

using namespace std;

long balance = 0;
mutex m;
condition_variable cv;

void addMoney(int money) {
    lock_guard<mutex> lg(m);
    balance += money;
    cout << "amount added current balance: " << balance << endl;
    cv.notify_one();
}

void withdrawMoney(int money) {
    unique_lock<mutex> ul(m); // you are locking the mutex m
    cv.wait(ul, [] {return (balance != 0) ? true : false; });
    if(balance >= money) {
        balance -= money;
        cout << "[I] amont deducted: " << money << endl;
    }
    else {
        cout << "[I] amount cannot be deducted, current balance is less than " << money << endl;
    }
    cout << "[I] current balance is " << balance << endl;
}

int main() {
    thread t0(withdrawMoney, 500);
    thread t1(addMoney, 400);
    t0.join();
    t1.join();
    return 0;
}