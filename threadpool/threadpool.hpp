#ifndef THREAD_POOL_H
#define THREAD_POOL_H

#include <vector>
#include <queue>
#include <memory> // std::make_shared

#include <thread> // std::thread
#include <mutex> // std::mutex, std::unique_lock
#include <condition_variable> // std::condition_variable
#include <future> // std::future, std::packaged_task

#include <functional> // std::function, std::bind
#include <stdexcept> // std:: runtime_error
#include <utility> // std::move, std::forward

using namespace std;

class ThreadPool {
private:
    vector<thread> workers_;
    // queue task, the type of queue elements are functions with void return type
    queue<function<void ()>> tasks_;
    // for synchronization
    mutex queue_mutex_;
    condition_variable cond_var_; // a new feature from c++11
    bool stop_;
public:
    ThreadPool(size_t); // initialize the number of concurrent threads
    
    // enqueue new thread task
    template<class F, class... Args>
    deltype(auto) enqueue(F&& f, Args&&... args);
    
    // destroy thread pool and all created threads
    ~ThreadPool();
};

inline ThreadPool::ThreadPool(size_t threads) : stop_(false) {
    // initialize worker
    for(size_t i = 0; i < threads; ++i) {

        
    }
}
#endif