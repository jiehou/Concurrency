#include <iostream>
#include <thread>

using std::cout;
using std::endl;
using std::thread;

void threadFunc() {
    for(int i = 0; i < 10; ++i) {
        cout << "thread function executing: " << i << endl;
    }
}

int main() {
    thread tobj(threadFunc);
    for(int i = 0; i < 10; ++i) {
        cout << "[I] display from MainThread: " << i << endl;
    }
    tobj.join();
    return 0;
}