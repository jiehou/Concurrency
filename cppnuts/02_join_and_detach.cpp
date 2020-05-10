#include <iostream>
#include <chrono>
#include <thread>

using namespace std;

void run(int count) {
    while (count-- > 0) {
        cout << "run count: " << count << endl;
    }
    this_thread::sleep_for(chrono::seconds(5)); // simulate this thread needs 5s to perform computations
    cout << "run finished computation" << endl;
}

int main() {
    thread t1(run, 10);
    cout << "main()" << endl;
    //t1.join();

    /*
    if(t1.joinable()) {
        cout << "[D] joinable" << endl;
        t1.join();
    }
    */
    t1.detach();
    cout << "main() after" << endl;
}