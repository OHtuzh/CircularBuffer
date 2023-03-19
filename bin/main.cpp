#include <iostream>
#include <ranges>
#include <vector>
#include "lib/CircularBufferExt.hpp"


int main() {
    CircularBufferExt<std::string> cb;

    cb.assign({"qwe", "abc", "def"});

    for (auto& v: cb) {
        std::cout << v << ' ';
    }
    std::cout << std::endl;

    cb.assign(10, "AHAHAHAH");
    for (auto& v: cb) {
        std::cout << v << ' ';
    }
    return 0;
}
