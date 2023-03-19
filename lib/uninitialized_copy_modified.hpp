#pragma once

#include <memory>

template<typename InputIterator, typename T, typename Alloc>
void my_uninitialized_copy(InputIterator start, InputIterator end, T* out, Alloc& allocator) {
    auto current = out;
    try {
        for (; start != end ; ++start, ++current) {
            std::allocator_traits<Alloc>::construct(allocator, current, *start);
        }
    } catch (...) {
        for (; out != current; ++out) {
            std::allocator_traits<Alloc>::destroy(allocator, out);
        }
        throw;
    }
}

template<typename T, typename Alloc>
void my_uninitialized_copy(std::size_t n, const T& value, T* out, Alloc& allocator) {
    auto current = out;
    std::size_t i = 0;
    try {
        for (; i < n ; ++i, ++current) {
            std::allocator_traits<Alloc>::construct(allocator, current, value);
        }
    } catch (...) {
        for (; out != current; ++out) {
            std::allocator_traits<Alloc>::destroy(allocator, out);
        }
        throw;
    }
}

template<typename InputIterator, typename T, typename Alloc>
void my_uninitialized_move(InputIterator start, InputIterator end, T* out, Alloc& allocator) {
    auto current = out;
    try {
        for (; start != end ; ++start, ++current) {
            std::allocator_traits<Alloc>::construct(allocator, current, std::move_if_noexcept(*start));
        }
    } catch (...) {
        for (; out != current; ++out) {
            std::allocator_traits<Alloc>::destroy(allocator, out);
        }
        throw;
    }
}

