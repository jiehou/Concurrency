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