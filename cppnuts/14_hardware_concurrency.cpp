#include <iostream>
#include <thread>

int main() {
    unsigned int n = std::thread::hardware_concurrency();
    std::cout << "[I] hardware support threads: " << n << std::endl;
    return 0;
}