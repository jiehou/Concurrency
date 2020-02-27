#include <iostream>
#include <queue>
#include <thread>
#include <mutex>
#include <chrono>

using namespace std;
mutex mu;
queue<int> q;

// producer: it generates and pushes data onto queue
void func1() {
    int count = 10;
    while(count > 0) {
        unique_lock<mutex> mlock(mu);
        cout << "[I] producer: " << count << endl;
        q.push(count);
        mlock.unlock();
        this_thread::sleep_for(chrono::seconds(2));
        count--;
    }
} 

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
        else
            mlock.unlock();
    } // busy checking
}

int main() {
    cout << "[I] producer and consumer problem \n";
    thread t1(func1);
    thread t2(func2);
    t1.join();
    t2.join();
    return 0;
}