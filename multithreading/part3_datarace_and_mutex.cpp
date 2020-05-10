#include <iostream>
#include <string>
#include <mutex>
#include <thread>

using namespace std;
mutex mu;

/* Note
* 1) not use mutex: lock and unlock directly, use lock_guard<mutex> 
*/
void SharedPrint(const string& msg, int num) {
    /*
    mu.lock();
    cout << msg << num << endl;
    mu.unlock();
    */
   lock_guard<mutex> lg(mu);
   cout << msg << num << endl;   
}

void Function1() {
    for(int i = 0; i < 100; ++i) {
        SharedPrint("From Function1: ", i);
    }
}
int main() {
    thread t1(Function1);
    for(int i = 0; i < 100; ++i) {
        SharedPrint("From main: ", i);
    }
    t1.join();
    return 0;
}