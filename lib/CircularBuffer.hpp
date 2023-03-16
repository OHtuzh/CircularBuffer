#pragma once

#include "CircularBufferTraits.hpp"

template<typename T, typename Alloc = std::allocator<T>>
class CircularBuffer : protected CircularBufferTraits<T, Alloc> {
public:
    USING_FIELDS;

    explicit CircularBuffer(const Alloc& allocator = Alloc());

    explicit CircularBuffer(CircularBufferTraits<T, Alloc>::size_type n, const Alloc& allocator = Alloc());

    CircularBuffer(CircularBufferTraits<T, Alloc>::size_type n,
                   CircularBufferTraits<T, Alloc>::value_type value,
                   const Alloc& allocator = Alloc());

    CircularBuffer(const CircularBuffer<T, Alloc>& other)
            :
            CircularBufferTraits<T, Alloc>(other) {}

    CircularBuffer(CircularBuffer<T, Alloc>&& other) noexcept
            :
            CircularBufferTraits<T, Alloc>(std::move(other)) {}

    ~CircularBuffer() {
        for (auto it = cbegin(); it != cend(); ++it) {
            AllocTraits::destroy(*this, std::addressof(*it));
        }

        AllocTraits::deallocate(*this, buff_start_, capacity() + 1);
    }

    CircularBuffer& operator=(const CircularBuffer& other) {
        static_cast<CircularBufferTraits<T, Alloc>&>(*this).operator=(
                static_cast<CircularBufferTraits<T, Alloc>&>(other));
        return *this;
    }

    CircularBuffer& operator=(CircularBuffer&& other) noexcept {
        static_cast<CircularBufferTraits<T, Alloc>&>(*this).operator=(
                std::move(static_cast<CircularBufferTraits<T, Alloc>&>(other)));
        return *this;
    }

    void push_back(const T& value);

    void push_back(T&& value);

    template<typename... Args>
    void emplace_back(Args&& ... args);

    void push_front(const T& value);

    void push_front(T&& value);

    template<typename... Args>
    void emplace_front(Args&& ... args);

private:
    using CircularBufferTraits<T, Alloc>::buff_start_;
    using CircularBufferTraits<T, Alloc>::buff_end_;
    using CircularBufferTraits<T, Alloc>::actual_start_;
    using CircularBufferTraits<T, Alloc>::actual_end_;
};

template<typename T, typename Alloc>
CircularBuffer<T, Alloc>::CircularBuffer(const Alloc& allocator)
        :
        CircularBufferTraits<T, Alloc>(allocator) {}


template<typename T, typename Alloc>
CircularBuffer<T, Alloc>::CircularBuffer(CircularBufferTraits<T, Alloc>::size_type n, const Alloc& allocator)
        :
        CircularBufferTraits<T, Alloc>(n, allocator) {}


template<typename T, typename Alloc>
CircularBuffer<T, Alloc>::CircularBuffer(CircularBufferTraits<T, Alloc>::size_type n,
                                         CircularBufferTraits<T, Alloc>::value_type value,
                                         const Alloc& allocator)
        :
        CircularBufferTraits<T, Alloc>(n, value, allocator) {}

template<typename T, typename Alloc>
void CircularBuffer<T, Alloc>::push_back(const T& value) {
    if (capacity() == 0) {
        return;
    }
    AllocTraits::construct(*this, actual_end_, value);
    auto next = (actual_end_ + 1 == buff_end_ ? buff_start_ : actual_end_ + 1);
    if (next == actual_start_) {
        AllocTraits::destroy(*this, actual_start_);
        actual_start_ = (actual_start_ + 1 == buff_end_ ? buff_start_ : actual_start_ + 1);
    }
    actual_end_ = (actual_end_ + 1 == buff_end_ ? buff_start_ : actual_end_ + 1);
}

template<typename T, typename Alloc>
void CircularBuffer<T, Alloc>::push_back(T&& value) {
    if (capacity() == 0) {
        return;
    }
    AllocTraits::construct(*this, actual_end_, std::move(value));
    auto next = (actual_end_ + 1 == buff_end_ ? buff_start_ : actual_end_ + 1);
    if (next == actual_start_) {
        AllocTraits::destroy(*this, actual_start_);
        actual_start_ = (actual_start_ + 1 == buff_end_ ? buff_start_ : actual_start_ + 1);
    }
    actual_end_ = next;
}

template<typename T, typename Alloc>
template<typename... Args>
void CircularBuffer<T, Alloc>::emplace_back(Args&& ... args) {
    if (capacity() == 0) {
        return;
    }
    AllocTraits::construct(*this, actual_end_, value_type(std::forward<Args>(args)...));
    auto next = (actual_end_ + 1 == buff_end_ ? buff_start_ : actual_end_ + 1);
    if (next == actual_start_) {
        AllocTraits::destroy(*this, actual_start_);
        actual_start_ = (actual_start_ + 1 == buff_end_ ? buff_start_ : actual_start_ + 1);
    }
    actual_end_ = (actual_end_ + 1 == buff_end_ ? buff_start_ : actual_end_ + 1);
}

template<typename T, typename Alloc>
void CircularBuffer<T, Alloc>::push_front(const T& value) {
    if (capacity() == 0) {
        return;
    }
    auto new_start = (actual_start_ == buff_start_ ? buff_end_ - 1 : actual_start_ - 1);
    AllocTraits::construct(*this, new_start, value);
    if (new_start == actual_end_) {
        actual_end_ = (actual_end_ == buff_start_ ? buff_end_ - 1 : actual_end_ - 1);
        AllocTraits::destroy(*this, actual_end_);
    }
    actual_start_ = new_start;
}

template<typename T, typename Alloc>
void CircularBuffer<T, Alloc>::push_front(T&& value) {
    if (capacity() == 0) {
        return;
    }
    auto new_start = (actual_start_ == buff_start_ ? buff_end_ - 1 : actual_start_ - 1);
    AllocTraits::construct(*this, new_start, std::move(value));
    if (new_start == actual_end_) {
        actual_end_ = (actual_end_ == buff_start_ ? buff_end_ - 1 : actual_end_ - 1);
        AllocTraits::destroy(*this, actual_end_);
    }
    actual_start_ = new_start;
}

template<typename T, typename Alloc>
template<typename... Args>
void CircularBuffer<T, Alloc>::emplace_front(Args&& ... args) {
    if (capacity() == 0) {
        return;
    }
    auto new_start = (actual_start_ == buff_start_ ? buff_end_ - 1 : actual_start_ - 1);
    AllocTraits::construct(*this, new_start, value_type(std::forward<Args>(args)...));
    if (new_start == actual_end_) {
        actual_end_ = (actual_end_ == buff_start_ ? buff_end_ - 1 : actual_end_ - 1);
        AllocTraits::destroy(*this, actual_end_);
    }
    actual_start_ = new_start;
}

