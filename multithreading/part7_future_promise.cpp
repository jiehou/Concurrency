#include <iostream>
#include <future>
#include <thread>

using namespace std;

void factorial(int n) {
    long long res = 1;
    for(int i = 1; i <= n; ++i)
        res *= i;
    cout << "[I] res: " << res << endl;
}

long long factorialRes(int n) {
    long long res = 1;
    for(int i = 1; i <= n; ++i)
        res *= i;
    return res;  
}

long long factorialFut(future<int>& fut) {
    long long res = 1;
    int n = fut.get(); // get n
    for(int i = 1;i <= n; ++i)
        res *= i;
    return res;
}

long long factorialSfut(shared_future<int>& sfut) {
    long long res = 1;
    int n = sfut.get(); // get n
    for(int i = 1;i <= n; ++i)
        res *= i;
    return res;
}

int main() {
    /*
    thread t1(factorial, 8);
    t1.join();
    */
    
    /*
    * async: 
    * 1) with param launch:async, create a new thread
    * 2) with param launch:deferred, use the same thread to compute result only when it is needed
    */
    // pass a value from child thread to parent thread
    future<long long> fut = async(launch::async, factorialRes, 8); // object to get result from future
    long long res = fut.get(); // get() can ONLY be called once
    cout << "[I] factorial(8): "<< res << endl;

    // pass a value from parent thread to child thread at sometime in the future
    promise<int> p;
    future<int> fut1 = p.get_future();
    future<long long> fut2 = async(launch::async, factorialFut, ref(fut1));
    p.set_value(8);
    cout << "[I] factorial(8): " << fut2.get() << endl;

    // shared future: broadcast
    promise<int> p1;
    future<int> fut3 = p1.get_future();
    shared_future<int> sf = fut3.share();
    future<long long> fut4 = async(launch::async, factorialSfut, ref(sf));
    future<long long> fut5 = async(launch::async, factorialSfut, ref(sf));
    p1.set_value(8);
    cout << "[I] factorial(8): " << fut4.get() << ", " << fut5.get() << endl;
    return 0;
}