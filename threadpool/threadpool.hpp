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
    ThreadPool(size_t threads); // initialize the number of concurrent threads
    // enqueue new thread task
    template<class F, class... Args>
    decltype(auto) enqueue(F&& f, Args&&... args); //@NOTE: deltype(auto)
    // destroy thread pool and all created threads
    ~ThreadPool();
};

// construct a fixed size of workers
inline ThreadPool::ThreadPool(size_t threads) : stop_(false) {
    // initialize workers
    for(size_t i = 0; i < threads; ++i) {
        workers_.emplace_back([this] { // lambda
            for(;;) {
                function<void()> task; // return value is void
                // critical section
                {
                    unique_lock<mutex> mlock(queue_mutex_);
                    this->cond_var_.wait(mlock, [this] {return this->stop_ || !this->tasks_.empty();});
                    if(this->stop_ || this->tasks_.empty()) return;
                    task = move(this->tasks_.front());
                    this->tasks_.pop();
                }
                // execute task
                task();
            }// endless for
        });
    }//for
}// constructor

// enqueue a new task
// use variadic templates and tail return type
template<class F, class... Args>
decltype(auto) ThreadPool::enqueue(F&& f, Args&&... args) {
    // deduce return type
    using return_typ = typename result_of<F(Args...)>::type;
    // fetch task: shared_ptr
    auto task = make_shared<packaged_task<return_typ()>> (
        bind(forward<F>(f), forward<Args>(args)...)); // @NOTE: forward
    future<return_typ> res = task->get_future();
    {
        unique_lock<mutex> mlock(queue_mutex_);
        // avoid adding new thread if threadpool is destroyed
        if(stop_)
            throw runtime_error("[E] enqueue on stopped threadpool");
        // add thread to queue
        tasks_.emplace([task]{ (*task)(); });
    }// critical section
    cond_var_.notify_one();
    return res;
}

// destroy everything
inline ThreadPool::~ThreadPool() {
    // critical section
    {
        unique_lock<mutex> mlock(queue_mutex_);
        stop_ = true;
    }
    // wake up all threads
    cond_var_.notify_all();
    for(auto& worker : workers_)
        worker.join();
}
#endif