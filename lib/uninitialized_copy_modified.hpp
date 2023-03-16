#pragma once

#include <memory>

template<typename InputIterator, typename T, typename Alloc>
void my_uninitialized_copy(InputIterator start, InputIterator end, T* out, Alloc& allocator) {
    auto current = out;
    try {
        for (; start != end ; ++start, ++current) {
            std::allocator_traits<Alloc>::construct(allocator, current, std::move(*start));
        }
    } catch (...) {
        for (; out != current; ++out) {
            std::allocator_traits<Alloc>::destroy(allocator, out);
        }
        throw;
    }
}

