#include <iostream>
#include <thread>
#include <string>
#include <memory>

using std::cout;
using std::endl;
using std::thread;
using std::string;
using std::move;
using std::ref;

void threadFunc() {
    for(int i = 0; i < 10; ++i) {
        cout << "thread function executing: " << i << endl;
    }
}

class Fctor {
public:
    void operator()(string& msg) {
        cout << "[I] Fctor: " << msg << endl;
    }
};

int main() {
    thread tobj(threadFunc);
    for(int i = 0; i < 10; ++i) {
        cout << "[I] display from MainThread: " << i << endl;
    }
    tobj.join();

    string s = "Where there is no trust, there is no love";
    thread t1{Fctor(), ref(s)};
    thread t2 = move(t1); // NOTE: use move to transfer ownership of thread
    t2.join();
    return 0;
}

/* Short notes
* 1) use move to transfer the ownership of thread
* 2) if(t1.joinable()) t1.join();
* 3) get the actual number of threads that the deployed hardware supports: 
     thread::hardware_concurrency();
* 4) std::ref: for some function that has reference parameter
*/