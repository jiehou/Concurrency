#include <iostream>
#include <future>
#include <mutex>
#include <condition_variable>
#include <functional>
#include <cmath>

using std::cout;
using std::endl;

int f(int x, int y) { return std::pow(x, y); }
using task_type = int(int, int);

void TaskLambda() {
    std::packaged_task<task_type> task([](int a, int b) { return std::pow(a, b); });
    std::future<int> res = task.get_future();
    task(2, 9); 
    cout << "[I] TaskLambda: " << res.get() << endl;
}

void TaskBind() {
    std::packaged_task<int()> task(std::bind(f, 2, 9));
    std::future<int> res = task.get_future();
    task();
    cout << "[I] TaskBind: " << res.get() << endl;
}

void TestPackagedTask() {
    /**
     * template<class R, class ...Args>
     * class packaged_task<R(Args...)>
     * thus, we cannot initialize a packaged_task as follows
    */
    //std::packaged_task<int(int, int)> task(f, 2, 9);
    //std::future<int> res = task.get_future();
    //cout << "[I] TestPackagedTask: " << res.get() << endl;
}

void TaskThread() {
    std::packaged_task<task_type> task(f);
    std::future<int> res = task.get_future();
    std::thread task_mt(std::move(task), 2, 9);
    task_mt.join();
    cout << "[I] TaskThread: " << res.get() << endl;
}

int main() {
    TaskLambda();
    TaskBind();
    TaskThread();
    return 0;
}
