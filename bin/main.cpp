#include <iostream>
#include <vector>
#include <list>
#include "../lib/CircularBuffer/CircularBuffer.hpp"
#include "../lib/CircularBufferExt/CircularBufferExt.hpp"
#include <deque>

int main() {
    std::vector<const int> v;
    CircularBuffer<int> a;
    CircularBuffer<int> b;
    b = a;
}
