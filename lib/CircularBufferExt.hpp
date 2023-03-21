#pragma once

#include "CircularBufferTraits.hpp"

template<typename T, typename Alloc = std::allocator<T>>
class CircularBufferExt : protected CircularBufferTraits<T, Alloc> {
public:
    USING_FIELDS;

    explicit CircularBufferExt(const Alloc& allocator = Alloc());

    explicit CircularBufferExt(CircularBufferTraits<T, Alloc>::size_type n, const Alloc& allocator = Alloc());

    CircularBufferExt(CircularBufferTraits<T, Alloc>::size_type n,
                      CircularBufferTraits<T, Alloc>::value_type value,
                      const Alloc& allocator = Alloc());

    CircularBufferExt(const CircularBufferExt<T, Alloc>& other)
            :
            CircularBufferTraits<T, Alloc>(other) {}

    CircularBufferExt(CircularBufferExt<T, Alloc>&& other) noexcept
            :
            CircularBufferTraits<T, Alloc>(std::move(other)) {}

    template<typename LegacyInputIterator>
    CircularBufferExt(LegacyInputIterator i, LegacyInputIterator j, const Alloc& allocator = Alloc())
            :
            CircularBufferTraits<T, Alloc>(i, j, allocator) {}

    CircularBufferExt(const std::initializer_list<value_type>& list, const Alloc& allocator = Alloc())
            :
            CircularBufferTraits<T, Alloc>(list, allocator) {}

    ~CircularBufferExt() {
        clear();

        AllocTraits::deallocate(*this, buff_start_, capacity() + 1);
    }

    CircularBufferExt& operator=(const CircularBufferExt& other) {
        static_cast<CircularBufferTraits<T, Alloc>&>(*this).operator=(
                static_cast<CircularBufferTraits<T, Alloc>&>(other));
        return *this;
    }

    CircularBufferExt& operator=(CircularBufferExt&& other) noexcept {
        static_cast<CircularBufferTraits<T, Alloc>&>(*this).operator=(
                std::move(static_cast<CircularBufferTraits<T, Alloc>&>(other)));
        return *this;
    }

    void swap(CircularBufferExt& other) {
        static_cast<CircularBufferTraits<T, Alloc>&>(*this).swap(static_cast<CircularBufferTraits<T, Alloc>&>(other));
    }

    void push_back(const T& value);

    void push_back(T&& value);

    template<typename... Args>
    void emplace_back(Args&& ... args);

    void push_front(const T& value);

    void push_front(T&& value);

    template<typename... Args>
    void emplace_front(Args&& ... args);

    iterator insert(const_iterator p, const_reference value);

    iterator insert(const_iterator p, value_type&& rv);

    iterator insert(const_iterator p, size_type n, const_reference value);

    template<typename... Args>
    iterator emplace(const_iterator p, Args&& ... args);

    template<typename LegacyInputIterator>
    requires std::input_iterator<LegacyInputIterator>
    iterator insert(const_iterator p, LegacyInputIterator i, LegacyInputIterator j);

    iterator insert(const_iterator p, const std::initializer_list<value_type>& il);

    bool operator==(const CircularBufferExt& other) const noexcept;

    bool operator!=(const CircularBufferExt& other) const noexcept;

private:
    using CircularBufferTraits<T, Alloc>::buff_start_;
    using CircularBufferTraits<T, Alloc>::buff_end_;
    using CircularBufferTraits<T, Alloc>::actual_start_;
    using CircularBufferTraits<T, Alloc>::actual_end_;
};

template<typename T, typename Alloc>
CircularBufferExt<T, Alloc>::CircularBufferExt(const Alloc& allocator)
        :
        CircularBufferTraits<T, Alloc>(allocator) {}


template<typename T, typename Alloc>
CircularBufferExt<T, Alloc>::CircularBufferExt(CircularBufferTraits<T, Alloc>::size_type n, const Alloc& allocator)
        :
        CircularBufferTraits<T, Alloc>(n, allocator) {}


template<typename T, typename Alloc>
CircularBufferExt<T, Alloc>::CircularBufferExt(CircularBufferTraits<T, Alloc>::size_type n,
                                               CircularBufferTraits<T, Alloc>::value_type value,
                                               const Alloc& allocator)
        :
        CircularBufferTraits<T, Alloc>(n, value, allocator) {}

template<typename T, typename Alloc>
void CircularBufferExt<T, Alloc>::push_back(const T& value) {
    if (size() == capacity()) {
        reserve(capacity() == 0 ? 1 : 2 * capacity());
    }
    AllocTraits::construct(*this, actual_end_, value);
    actual_end_ = (actual_end_ + 1 == buff_end_ ? buff_start_ : actual_end_ + 1);
}

template<typename T, typename Alloc>
void CircularBufferExt<T, Alloc>::push_back(T&& value) {
    if (size() == capacity()) {
        reserve(capacity() == 0 ? 1 : 2 * capacity());
    }
    AllocTraits::construct(*this, actual_end_, std::move(value));
    actual_end_ = (actual_end_ + 1 == buff_end_ ? buff_start_ : actual_end_ + 1);
}

template<typename T, typename Alloc>
template<typename... Args>
void CircularBufferExt<T, Alloc>::emplace_back(Args&& ... args) {
    if (size() == capacity()) {
        reserve(capacity() == 0 ? 1 : 2 * capacity());
    }
    AllocTraits::construct(*this, actual_end_, value_type(std::forward<Args>(args)...));
    actual_end_ = (actual_end_ + 1 == buff_end_ ? buff_start_ : actual_end_ + 1);
}

template<typename T, typename Alloc>
void CircularBufferExt<T, Alloc>::push_front(const T& value) {
    if (size() == capacity()) {
        reserve(capacity() == 0 ? 1 : 2 * capacity());
    }
    auto new_start = (actual_start_ == buff_start_ ? buff_end_ - 1 : actual_start_ - 1);
    AllocTraits::construct(*this, new_start, value);
    actual_start_ = new_start;
}

template<typename T, typename Alloc>
void CircularBufferExt<T, Alloc>::push_front(T&& value) {
    if (size() == capacity()) {
        reserve(capacity() == 0 ? 1 : 2 * capacity());
    }
    auto new_start = (actual_start_ == buff_start_ ? buff_end_ - 1 : actual_start_ - 1);
    AllocTraits::construct(*this, new_start, std::move(value));
    actual_start_ = new_start;
}

template<typename T, typename Alloc>
template<typename... Args>
void CircularBufferExt<T, Alloc>::emplace_front(Args&& ... args) {
    if (size() == capacity()) {
        reserve(capacity() == 0 ? 1 : 2 * capacity());
    }
    auto new_start = (actual_start_ == buff_start_ ? buff_end_ - 1 : actual_start_ - 1);
    AllocTraits::construct(*this, new_start, value_type(std::forward<Args>(args)...));
    actual_start_ = new_start;
}

template<typename T, typename Alloc>
CircularBufferExt<T, Alloc>::iterator
CircularBufferExt<T, Alloc>::insert(CircularBufferExt<T, Alloc>::const_iterator p, const_reference value) {
    if (std::addressof(*p) < buff_start_ || std::addressof(*p) >= buff_end_) {
        throw std::out_of_range("Iterator is out of bounds");
    }
    size_type index = p - begin();
    if (index > size()) {
        throw std::out_of_range("Iterator is out of bounds");
    }

    if (size() == capacity()) {
        reserve(capacity() == 0 ? 1 : 2 * capacity());
    }
    if (index == size()) {
        push_back(value);
        return --end();
    }
    if (index == 0) {
        push_front(value);
        return begin();
    }

    auto last = --end();
    p = begin() + index;
    AllocTraits::construct(*this, actual_end_, std::move_if_noexcept(*last));
    actual_end_ = (actual_end_ + 1 == buff_end_ ? buff_start_ : actual_end_ + 1);
    for (; last != p; --last) {
        *last = std::move_if_noexcept(*(last - 1));
    }
    *p = value;
    return p;
}

template<typename T, typename Alloc>
CircularBufferExt<T, Alloc>::iterator
CircularBufferExt<T, Alloc>::insert(CircularBufferExt<T, Alloc>::const_iterator p, value_type&& rv) {
    if (std::addressof(*p) < buff_start_ || std::addressof(*p) >= buff_end_) {
        throw std::out_of_range("Iterator is out of bounds");
    }
    size_type index = p - begin();
    if (index > size()) {
        throw std::out_of_range("Iterator is out of bounds");
    }

    if (size() == capacity()) {
        reserve(capacity() == 0 ? 1 : 2 * capacity());
    }
    if (index == size()) {
        push_back(std::move(rv));
        return --end();
    }
    if (index == 0) {
        push_front(std::move(rv));
        return begin();
    }
    auto last = --end();
    auto it = begin() + index;
    AllocTraits::construct(*this, actual_end_, std::move_if_noexcept(*last));
    actual_end_ = (actual_end_ + 1 == buff_end_ ? buff_start_ : actual_end_ + 1);
    for (; last != it; --last) {
        *last = std::move_if_noexcept(*(last - 1));
    }
    *it = std::move_if_noexcept(rv);
    return it;
}


template<typename T, typename Alloc>
CircularBufferExt<T, Alloc>::iterator
CircularBufferExt<T, Alloc>::insert(CircularBufferExt<T, Alloc>::const_iterator p, CircularBufferExt<T, Alloc>::size_type n,
                                    const_reference value) {
    if (std::addressof(*p) < buff_start_ || std::addressof(*p) >= buff_end_) {
        throw std::out_of_range("Iterator is out of bounds");
    }
    size_type index = p - begin();
    if (n == 0) {
        return begin() + index;
    }
    if (index > size()) {
        throw std::out_of_range("Iterator is out of bounds");
    }

    size_type target_capacity = capacity();
    while (target_capacity < size() + n) {
        target_capacity *= 2;
    }
    reserve(target_capacity);
    if (index == size()) {
        auto it = end();
        for (size_type i = 0; i < n; ++i) {
            push_back(value);
        }
        return it;
    }
    if (index == 0) {
        for (size_type i = 0; i < n; ++i) {
            push_front(value);
        }
        return begin();
    }

    actual_end_ = (actual_end_ + n >= buff_end_ ? actual_end_ = buff_start_ + ((actual_end_ + n) - buff_end_) :
                   actual_end_ + n);

    auto to_insert = begin() + index;
    for (auto it = end() - n; it != to_insert; --it) {
        AllocTraits::construct(*this, std::addressof(*(it + n - 1)), std::move_if_noexcept(*(it - 1)));
    }
    for (size_type i = 0; i < n; ++i) {
        AllocTraits::construct(*this, std::addressof(to_insert[i]), value);
    }

    return to_insert;
}

template<typename T, typename Alloc>
template<typename... Args>
CircularBufferExt<T, Alloc>::iterator
CircularBufferExt<T, Alloc>::emplace(CircularBufferExt<T, Alloc>::const_iterator p, Args&& ... args) {
    if (std::addressof(*p) < buff_start_ || std::addressof(*p) >= buff_end_) {
        throw std::out_of_range("Iterator is out of bounds");
    }
    size_type index = p - begin();
    if (index > size()) {
        throw std::out_of_range("Iterator is out of bounds");
    }

    if (size() == capacity()) {
        reserve(capacity() == 0 ? 1 : 2 * capacity());
    }

    if (index == size()) {
        emplace_back(std::forward<Args>(args)...);
        return --end();
    }
    if (index == 0) {
        push_front(std::forward<Args>(args)...);
        return begin();
    }
    auto last = --end();
    p = begin() + index;
    AllocTraits::construct(*this, actual_end_, std::move_if_noexcept(*last));
    actual_end_ = (actual_end_ + 1 == buff_end_ ? buff_start_ : actual_end_ + 1);
    for (; last != p; --last) {
        *last = std::move_if_noexcept(*(last - 1));
    }
    *p = value_type(std::forward<Args>(args)...);
    return p;
}

template<typename T, typename Alloc>
template<typename LegacyInputIterator>
requires std::input_iterator<LegacyInputIterator>
CircularBufferExt<T, Alloc>::iterator
CircularBufferExt<T, Alloc>::insert(CircularBufferExt<T, Alloc>::const_iterator p, LegacyInputIterator i,
                                    LegacyInputIterator j) {
    if (std::addressof(*p) < buff_start_ || std::addressof(*p) >= buff_end_) {
        throw std::out_of_range("Iterator is out of bounds");
    }
    size_type n = std::distance(i, j);
    size_type index = p - begin();
    if (n == 0) {
        return begin() + index;
    }
    if (index > size()) {
        throw std::out_of_range("Iterator is out of bounds");
    }

    size_type target_capacity = capacity();
    while (target_capacity < size() + n) {
        target_capacity *= 2;
    }
    reserve(target_capacity);

    if (index == size()) {
        auto it = end();
        for (; i != j; ++i) {
            push_back(*i);
        }
        return it;
    }
    if (index == 0) {
        for (; i != j; ++i) {
            push_front(*i);
        }
        return begin();
    }

    actual_end_ = (actual_end_ + n >= buff_end_ ? actual_end_ = buff_start_ + ((actual_end_ + n) - buff_end_) :
                   actual_end_ + n);

    auto to_insert = begin() + index;
    for (auto it = end() - n; it != to_insert; --it) {
        AllocTraits::construct(*this, std::addressof(*(it + n - 1)), std::move_if_noexcept(*(it - 1)));
    }
    for (size_type k = 0; k < n; ++i, ++k) {
        AllocTraits::construct(*this, std::addressof(to_insert[k]), *i);
    }

    return to_insert;
}

template<typename T, typename Alloc>
CircularBufferExt<T, Alloc>::iterator
CircularBufferExt<T, Alloc>::insert(CircularBufferExt<T, Alloc>::const_iterator p,
                                    const std::initializer_list<value_type>& il) {
    return insert(p, il.begin(), il.end());
}


template<typename T, typename Alloc>
bool CircularBufferExt<T, Alloc>::operator==(const CircularBufferExt& other) const noexcept {
    return static_cast<const CircularBufferTraits<T, Alloc>&>(*this).operator==(
            static_cast<const CircularBufferTraits<T, Alloc>&>(other));
}

template<typename T, typename Alloc>
bool CircularBufferExt<T, Alloc>::operator!=(const CircularBufferExt& other) const noexcept {
    return static_cast<const CircularBufferTraits<T, Alloc>&>(*this).operator!=(
            static_cast<const CircularBufferTraits<T, Alloc>&>(other));
}

template<typename T, typename Alloc>
void swap(CircularBufferExt<T, Alloc>& lhs, CircularBufferExt<T, Alloc>& rhs) {
    lhs.swap(rhs);
}

#undef USING_FIELDS
