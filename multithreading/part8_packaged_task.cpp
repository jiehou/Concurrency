#include <iostream>
#include <future>
#include <queue>
#include <mutex>
#include <condition_variable>
#include <functional>

using namespace std;

queue<packaged_task<int()>> taskq; // task queue: store task that returns int
mutex mu;
condition_variable cond;

int factorialRes(int n) {
    long long res = 1;
    for(int i = 1; i <= n; ++i)
        res *= i;
    return res;  
}

// packaged_task queue
void doWork() {
    packaged_task<int()> pt;
    {
        unique_lock<mutex> mlock;
        cond.wait(mlock, []() {return !taskq.empty();});
        pt = move(taskq.front());
        taskq.pop();
    }
    pt(); // launch the task
}

int main() {
    thread t1(doWork);
    packaged_task<int()> pt(bind(factorialRes, 8));
    future<int> fut = pt.get_future();
    {
        unique_lock<mutex> mlock(mu);
        taskq.push(move(pt));
    }// critical section
    cond.notify_one();
    t1.join();
    cout << "[I] factorial(10): " << fut.get() << endl;
    return 0;
}