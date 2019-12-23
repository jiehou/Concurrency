/**
 * 1) race condition: a situation where two or more threads/process happend to modify a common data at the same time
 * if there is a race condition, we have to protect it
 * 2) mutex: it is used to avoid race condition
 * we use lock and unlock to avoid race condition
*/ 
#include <iostream>
#include <thread>
#include <mutex>
using namespace std;

int myAccount = 0;
mutex m;

void addMoney() {
    m.lock();
    ++myAccount;
    m.unlock();
}

int main() {
    thread t1(addMoney);
    thread t2(addMoney);

    t1.join();
    cout << "[I] after t1: " << myAccount << endl;
    t2.join();
    cout << "[I] after t2: " << myAccount << endl;
    return 0;
}