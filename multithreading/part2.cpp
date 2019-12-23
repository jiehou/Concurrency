#include <iostream>
#include <thread>
#include <vector>
#include <algorithm>
#include <functional>
using namespace std;

class WorkerThread {
public:
    void operator()() {
        cout << "worker thread: " << this_thread::get_id() << endl;
    }
};

int main() {
    vector<thread> threadList;
    for(int i = 0; i < 10; ++i) {
        threadList.push_back(thread(WorkerThread()));
    }
    // wait for all the worker thread to finish
    cout << "[I] wait for all the worker threads to finish" << endl;
    for_each(threadList.begin(), threadList.end(),mem_fn(&thread::join));
    cout << "[I] exiting from MainThread" << endl;
}