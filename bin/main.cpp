#include <iostream>
#include <ranges>
#include <vector>
#include <list>
#include "lib/CircularBuffer.hpp"
#include "lib/CircularBufferExt.hpp"
#include <deque>

int id = 0;

class MyClass {
public:
    int my_id;
    MyClass() {
        my_id = id++;
        std::cout << "MyClass() " << my_id << std::endl;
    }

    MyClass(const MyClass& other) {
        my_id = other.my_id;
        std::cout << "MyClass(const MyClass& other) " << my_id << std::endl;
    }

    MyClass(MyClass&& other) {
        my_id = other.my_id;
        std::cout << "MyClass(MyClass&& other) " << my_id << std::endl;
    }

    ~MyClass() {
        std::cout << "~MyClass() " << my_id << std::endl;
    }
};

int main() {
    CircularBuffer<MyClass> cb;
    cb.reserve(5);

    for (int i = 0; i < 10; ++i) {
        cb.push_back(MyClass());
    }
    cb.reserve(10);
    for (int i = 0; i < 10; i++) {
        cb.emplace_back(MyClass());
    }
    std::cout << cb.size() << " | " << cb.capacity() << std::endl;
    return 0;
}
