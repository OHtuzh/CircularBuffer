#include <iostream>
#include <ranges>
#include <vector>
#include <list>
#include "../lib/CircularBuffer/CircularBuffer.hpp"
#include "../lib/CircularBufferExt/CircularBufferExt.hpp"
#include <deque>

int main() {
    CircularBuffer<int> a(10);
    for (int i = 0; i < 15; ++i) {
        a.push_front(i);
    }
    for (auto i: a) {
        std::cout << i << ' ';
    }
    std::cout << std::endl;
    for (int it : std::ranges::reverse_view(a)) {
        std::cout << it << ' ';
    }
}
