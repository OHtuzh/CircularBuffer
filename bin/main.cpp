#include <iostream>
#include <ranges>
#include <vector>
#include "lib/CircularBuffer.hpp"
#include "lib/CircularBufferExt.hpp"


int main() {
    std::vector<int> v;
    v.max_size();
    CircularBuffer<int> cb(5);
    for (int i = 0; i < 5; ++i) {
        if (i % 2 == 0) {
            cb.push_back(i);
        } else {
            cb.push_front(i);
        }
    }
    for (auto i: cb) {
        std::cout << i << ' ';
    }
    std::cout << std::endl;

    cb.erase(cb.cbegin() + 1, cb.cend() - 1);
    std::cout << *cb.begin() << std::endl;

    for (auto i: cb) {
        std::cout << i << ' ';
    }
    return 0;
}
