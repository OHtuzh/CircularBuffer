#pragma once

#include "CircularBufferBase.hpp"

template<typename T, std::size_t scale_factor = 2, typename Alloc = std::allocator<T>>
class CircularBufferExt : protected CircularBufferBase<T, Alloc> {
public:
    USING_FIELDS;

    explicit CircularBufferExt(const Alloc& allocator = Alloc()) : CircularBufferBase<T, Alloc>(allocator) {}

    explicit CircularBufferExt(CircularBufferBase<T, Alloc>::size_type n, const Alloc& allocator = Alloc())
            : CircularBufferBase<T, Alloc>(n, allocator) {}

    CircularBufferExt(CircularBufferBase<T, Alloc>::size_type n,
                      CircularBufferBase<T, Alloc>::value_type value,
                      const Alloc& allocator = Alloc()) : CircularBufferBase<T, Alloc>(n, value, allocator) {}

    CircularBufferExt(const CircularBufferExt<T, scale_factor, Alloc>& other) : CircularBufferBase<T, Alloc>(other) {}

    CircularBufferExt(CircularBufferExt<T, scale_factor, Alloc>&& other) noexcept: CircularBufferBase<T, Alloc>(std::move(other)) {}

    template<typename LegacyInputIterator>
    CircularBufferExt(LegacyInputIterator i, LegacyInputIterator j, const Alloc& allocator = Alloc())
            : CircularBufferBase<T, Alloc>(i, j, allocator) {}

    CircularBufferExt(const std::initializer_list<value_type>& list, const Alloc& allocator = Alloc())
            : CircularBufferBase<T, Alloc>(list, allocator) {}

    ~CircularBufferExt() {
        clear();
        AllocTraits::deallocate(allocator_, buff_start_, capacity() + 1);
    }

    CircularBufferExt& operator=(const CircularBufferExt& other) {
        static_cast<CircularBufferBase<T, Alloc>&>(*this).operator=(
                static_cast<CircularBufferBase<T, Alloc>&>(other));
        return *this;
    }

    CircularBufferExt& operator=(CircularBufferExt&& other) noexcept {
        static_cast<CircularBufferBase<T, Alloc>&>(*this).operator=(
                std::move(static_cast<CircularBufferBase<T, Alloc>&>(other)));
        return *this;
    }

    void swap(CircularBufferExt& other) {
        static_cast<CircularBufferBase<T, Alloc>&>(*this).swap(static_cast<CircularBufferBase<T, Alloc>&>(other));
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

protected:
    using CircularBufferBase<T, Alloc>::buff_start_;
    using CircularBufferBase<T, Alloc>::buff_end_;
    using CircularBufferBase<T, Alloc>::actual_start_;
    using CircularBufferBase<T, Alloc>::actual_end_;
    using CircularBufferBase<T, Alloc>::allocator_;

private:
    inline void reserve_if_full(size_type current_size, size_type current_capacity) {
        if (current_size == current_capacity) {
            reserve(current_capacity == 0 ? 1 : current_capacity * scale_factor);
        }
    }
};

template<typename T, std::size_t scale_factor, typename Alloc>
void CircularBufferExt<T, scale_factor, Alloc>::push_back(const T& value) {
    reserve_if_full(size(), capacity());
    AllocTraits::construct(allocator_, actual_end_, value);
    actual_end_ = (actual_end_ + 1 == buff_end_ ? buff_start_ : actual_end_ + 1);
}

template<typename T, std::size_t scale_factor, typename Alloc>
void CircularBufferExt<T, scale_factor, Alloc>::push_back(T&& value) {
    reserve_if_full(size(), capacity());
    AllocTraits::construct(allocator_, actual_end_, std::move(value));
    actual_end_ = (actual_end_ + 1 == buff_end_ ? buff_start_ : actual_end_ + 1);
}

template<typename T, std::size_t scale_factor, typename Alloc>
template<typename... Args>
void CircularBufferExt<T, scale_factor, Alloc>::emplace_back(Args&& ... args) {
    reserve_if_full(size(), capacity());
    AllocTraits::construct(allocator_, actual_end_, value_type(std::forward<Args>(args)...));
    actual_end_ = (actual_end_ + 1 == buff_end_ ? buff_start_ : actual_end_ + 1);
}

template<typename T, std::size_t scale_factor, typename Alloc>
void CircularBufferExt<T, scale_factor, Alloc>::push_front(const T& value) {
    reserve_if_full(size(), capacity());
    auto new_start = (actual_start_ == buff_start_ ? buff_end_ - 1 : actual_start_ - 1);
    AllocTraits::construct(allocator_, new_start, value);
    actual_start_ = new_start;
}

template<typename T, std::size_t scale_factor, typename Alloc>
void CircularBufferExt<T, scale_factor, Alloc>::push_front(T&& value) {
    reserve_if_full(size(), capacity());
    auto new_start = (actual_start_ == buff_start_ ? buff_end_ - 1 : actual_start_ - 1);
    AllocTraits::construct(allocator_, new_start, std::move(value));
    actual_start_ = new_start;
}

template<typename T, std::size_t scale_factor, typename Alloc>
template<typename... Args>
void CircularBufferExt<T, scale_factor, Alloc>::emplace_front(Args&& ... args) {
    reserve_if_full(size(), capacity());
    auto new_start = (actual_start_ == buff_start_ ? buff_end_ - 1 : actual_start_ - 1);
    AllocTraits::construct(allocator_, new_start, value_type(std::forward<Args>(args)...));
    actual_start_ = new_start;
}

template<typename T, std::size_t scale_factor, typename Alloc>
CircularBufferExt<T, scale_factor, Alloc>::iterator
CircularBufferExt<T, scale_factor, Alloc>::insert(CircularBufferExt<T, scale_factor, Alloc>::const_iterator p, const_reference value) {
    if (std::addressof(*p) < buff_start_ || std::addressof(*p) >= buff_end_) {
        throw std::out_of_range("Iterator is out of bounds");
    }
    size_type index = p - begin();
    if (index > size()) {
        throw std::out_of_range("Iterator is out of bounds");
    }

    reserve_if_full(size(), capacity());
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
    AllocTraits::construct(allocator_, actual_end_, std::move_if_noexcept(*last));
    actual_end_ = (actual_end_ + 1 == buff_end_ ? buff_start_ : actual_end_ + 1);
    for (; last != p; --last) {
        *last = std::move_if_noexcept(*(last - 1));
    }
    *p = value;
    return p;
}

template<typename T, std::size_t scale_factor, typename Alloc>
CircularBufferExt<T, scale_factor, Alloc>::iterator
CircularBufferExt<T, scale_factor, Alloc>::insert(CircularBufferExt<T, scale_factor, Alloc>::const_iterator p, value_type&& rv) {
    if (std::addressof(*p) < buff_start_ || std::addressof(*p) >= buff_end_) {
        throw std::out_of_range("Iterator is out of bounds");
    }
    size_type index = p - begin();
    if (index > size()) {
        throw std::out_of_range("Iterator is out of bounds");
    }

    reserve_if_full(size(), capacity());
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
    AllocTraits::construct(allocator_, actual_end_, std::move_if_noexcept(*last));
    actual_end_ = (actual_end_ + 1 == buff_end_ ? buff_start_ : actual_end_ + 1);
    for (; last != it; --last) {
        *last = std::move_if_noexcept(*(last - 1));
    }
    *it = std::move_if_noexcept(rv);
    return it;
}


template<typename T, std::size_t scale_factor, typename Alloc>
CircularBufferExt<T, scale_factor, Alloc>::iterator
CircularBufferExt<T, scale_factor, Alloc>::insert(CircularBufferExt<T, scale_factor, Alloc>::const_iterator p,
                                    CircularBufferExt<T, scale_factor, Alloc>::size_type n,
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
        AllocTraits::construct(allocator_, std::addressof(*(it + n - 1)), std::move_if_noexcept(*(it - 1)));
    }
    for (size_type i = 0; i < n; ++i) {
        AllocTraits::construct(allocator_, std::addressof(to_insert[i]), value);
    }

    return to_insert;
}

template<typename T, std::size_t scale_factor, typename Alloc>
template<typename... Args>
CircularBufferExt<T, scale_factor, Alloc>::iterator
CircularBufferExt<T, scale_factor, Alloc>::emplace(CircularBufferExt<T, scale_factor, Alloc>::const_iterator p, Args&& ... args) {
    if (std::addressof(*p) < buff_start_ || std::addressof(*p) >= buff_end_) {
        throw std::out_of_range("Iterator is out of bounds");
    }
    size_type index = p - begin();
    if (index > size()) {
        throw std::out_of_range("Iterator is out of bounds");
    }

    reserve_if_full(size(), capacity());

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
    AllocTraits::construct(allocator_, actual_end_, std::move_if_noexcept(*last));
    actual_end_ = (actual_end_ + 1 == buff_end_ ? buff_start_ : actual_end_ + 1);
    for (; last != p; --last) {
        *last = std::move_if_noexcept(*(last - 1));
    }
    *p = value_type(std::forward<Args>(args)...);
    return p;
}

template<typename T, std::size_t scale_factor, typename Alloc>
template<typename LegacyInputIterator>
requires std::input_iterator<LegacyInputIterator>
CircularBufferExt<T, scale_factor, Alloc>::iterator
CircularBufferExt<T, scale_factor, Alloc>::insert(CircularBufferExt<T, scale_factor, Alloc>::const_iterator p, LegacyInputIterator i,
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
        AllocTraits::construct(allocator_, std::addressof(*(it + n - 1)), std::move_if_noexcept(*(it - 1)));
    }
    for (size_type k = 0; k < n; ++i, ++k) {
        AllocTraits::construct(allocator_, std::addressof(to_insert[k]), *i);
    }

    return to_insert;
}

template<typename T, std::size_t scale_factor, typename Alloc>
CircularBufferExt<T, scale_factor, Alloc>::iterator
CircularBufferExt<T, scale_factor, Alloc>::insert(CircularBufferExt<T, scale_factor, Alloc>::const_iterator p,
                                    const std::initializer_list<value_type>& il) {
    return insert(p, il.begin(), il.end());
}


template<typename T, std::size_t scale_factor, typename Alloc>
bool CircularBufferExt<T, scale_factor, Alloc>::operator==(const CircularBufferExt& other) const noexcept {
    return static_cast<const CircularBufferBase<T, Alloc>&>(*this).operator==(
            static_cast<const CircularBufferBase<T, Alloc>&>(other));
}

template<typename T, std::size_t scale_factor, typename Alloc>
bool CircularBufferExt<T, scale_factor, Alloc>::operator!=(const CircularBufferExt& other) const noexcept {
    return static_cast<const CircularBufferBase<T, Alloc>&>(*this).operator!=(
            static_cast<const CircularBufferBase<T, Alloc>&>(other));
}

template<typename T, std::size_t scale_factor, typename Alloc>
void swap(CircularBufferExt<T, scale_factor, Alloc>& lhs, CircularBufferExt<T, scale_factor, Alloc>& rhs) {
    lhs.swap(rhs);
}

#undef USING_FIELDS
