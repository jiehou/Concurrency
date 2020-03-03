#include <iostream>
#include <vector>
#include "threadpool.hpp"

using namespace std;

int main() {
    ThreadPool pool(4);
    vector<future<int>> results;
    for(int i = 0; i < 8; ++i) {
        results.emplace_back(
            pool.enqueue([i] { // lambda
                //cout << "[I] value of i: " << i << endl;
                return i*i;
            })
        );
    }
    for (auto&& result:results)
        cout << result.get() << endl;
    cout << endl;
    return 0;
}