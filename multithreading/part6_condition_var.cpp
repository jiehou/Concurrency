#include <iostream>
#include <queue>
#include <thread>
#include <mutex>
#include <condition_variable>
#include <chrono>

using namespace std;
mutex mu;
queue<int> q;
condition_variable cond;

// producer: generates and pushes data onto queue
void func1() {
    int num = 10;
    while(num > 0) {
        unique_lock<mutex> mlock(mu);
        q.push(num);
        mlock.unlock();
        cout << "[I] producer: " << num << endl;
        this_thread::sleep_for(chrono::seconds(2));
        num--;
    }
}

// producer: uses condition_variable
void func1_cond() {
    int num = 10;
    while(num > 0) {
        unique_lock<mutex> mlock(mu); // first task is to acquire lock before access shared resource
        q.push(num);
        mlock.unlock(); // release acquired lock
        cout << "[I] producer: " << num << endl;
        // wake up the waiting thread
        cond.notify_one();  // notify_all()
        this_thread::sleep_for(chrono::seconds(2));
        num--;
    }
}

// consumer: uses busy-checking
void func2() {
    int data=-1;
    //cout << "[D] func2\n";
    while(data != 1) {
        unique_lock<mutex> mlock(mu);
        if(!q.empty()) {
            data = q.front();
            q.pop();
            mlock.unlock();
            cout << "[I] consumer: " << data << endl;
        }
        else {
            mlock.unlock();
            this_thread::sleep_for(chrono::milliseconds(10));
        }
    } // busy checking
    /*busy checking: not efficient*/
}

void func2_cond() {
    int data = -1;
    while(data != 1) {
        unique_lock<mutex> mlock(mu);
        cond.wait(mlock, []() { return !q.empty(); }); // once producer produces sth, consumer is waked up
        data = q.front();
        q.pop();
        mlock.unlock();
        cout << "[I] consumer: " << data << endl;
    }
}

int main() {
    cout << "[I] producer and consumer problem \n";
    //thread t1(func1);
    //thread t2(func2);
    thread t1(func1_cond);
    thread t2(func2_cond);
    t1.join();
    t2.join();
    return 0;
}