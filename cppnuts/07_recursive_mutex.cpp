#include <iostream>
#include <thread>
#include <mutex>
#include <string>
using namespace std;

recursive_mutex m1;
int buffer = 0;

void recursion(const string &s, int loopFor) {
    if(loopFor < 0) return;
    m1.lock();
    cout << s << ": " << buffer++ << endl;
    recursion(s, --loopFor);
    m1.unlock();
}

int main() {
    thread t1(recursion, "t1", 10);
    thread t2(recursion, "t2", 10);
    t1.join();
    t2.join();
    return 0;
}