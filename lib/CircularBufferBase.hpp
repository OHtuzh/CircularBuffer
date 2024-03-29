#pragma once

#include "Iterator.hpp"
#include "uninitialized_copy_modified.hpp"


#define USING_FIELDS \
    using typename CircularBufferBase<T, Alloc>::allocator_type; \
    using typename CircularBufferBase<T, Alloc>::AllocTraits; \
    using typename CircularBufferBase<T, Alloc>::iterator; \
    using typename CircularBufferBase<T, Alloc>::const_iterator; \
    using typename CircularBufferBase<T, Alloc>::reverse_iterator; \
    using typename CircularBufferBase<T, Alloc>::const_reverse_iterator; \
    using typename CircularBufferBase<T, Alloc>::value_type; \
    using typename CircularBufferBase<T, Alloc>::reference; \
    using typename CircularBufferBase<T, Alloc>::pointer; \
    using typename CircularBufferBase<T, Alloc>::const_reference; \
    using typename CircularBufferBase<T, Alloc>::difference_type; \
    using typename CircularBufferBase<T, Alloc>::size_type; \
    using CircularBufferBase<T, Alloc>::begin; \
    using CircularBufferBase<T, Alloc>::end; \
    using CircularBufferBase<T, Alloc>::rbegin; \
    using CircularBufferBase<T, Alloc>::rend; \
    using CircularBufferBase<T, Alloc>::cbegin; \
    using CircularBufferBase<T, Alloc>::cend; \
    using CircularBufferBase<T, Alloc>::crbegin; \
    using CircularBufferBase<T, Alloc>::swap; \
    using CircularBufferBase<T, Alloc>::size; \
    using CircularBufferBase<T, Alloc>::capacity; \
    using CircularBufferBase<T, Alloc>::max_size; \
    using CircularBufferBase<T, Alloc>::empty; \
    using CircularBufferBase<T, Alloc>::reserve; \
    using CircularBufferBase<T, Alloc>::resize; \
    using CircularBufferBase<T, Alloc>::erase; \
    using CircularBufferBase<T, Alloc>::clear; \
    using CircularBufferBase<T, Alloc>::assign; \
    using CircularBufferBase<T, Alloc>::pop_back; \
    using CircularBufferBase<T, Alloc>::pop_front; \
    using CircularBufferBase<T, Alloc>::front; \
    using CircularBufferBase<T, Alloc>::back; \
    using CircularBufferBase<T, Alloc>::get_allocator;


template<typename T, typename Alloc = std::allocator<T>>
class CircularBufferBase {
public:
    using allocator_type = typename std::allocator_traits<Alloc>::template rebind_alloc<T>;
    using AllocTraits = typename std::allocator_traits<Alloc>::template rebind_traits<T>;

    using iterator = CommonIterator<T>;
    using const_iterator = CommonIterator<const T>;
    using reverse_iterator = std::reverse_iterator<CommonIterator<T>>;
    using const_reverse_iterator = std::reverse_iterator<CommonIterator<const T>>;

    using value_type = T;
    using reference = T&;
    using const_reference = const T&;
    using pointer = T*;

    using difference_type = iterator::difference_type;
    using size_type = std::size_t;

    static_assert(std::random_access_iterator<iterator>, "Common iterator isn't random access iterator");

    void swap(CircularBufferBase& other);

    reference operator[](size_type i);

    const_reference operator[](size_type i) const;

    iterator begin() noexcept;

    iterator end() noexcept;

    const_iterator begin() const noexcept;

    const_iterator end() const noexcept;

    const_iterator cbegin() const noexcept;

    const_iterator cend() const noexcept;

    reverse_iterator rbegin() noexcept;

    reverse_iterator rend() noexcept;

    const_reverse_iterator rbegin() const noexcept;

    const_reverse_iterator rend() const noexcept;

    const_reverse_iterator crbegin() const noexcept;

    const_reverse_iterator crend() const noexcept;

    iterator erase(const_iterator q);

    iterator erase(const_iterator q1, const_iterator q2);

    void clear() noexcept;

    void assign(size_type n, const_reference value);

    template<typename LegacyInputIterator>
    requires std::input_iterator<LegacyInputIterator>
    void assign(LegacyInputIterator i, LegacyInputIterator j);

    void assign(const std::initializer_list<value_type>& il);

    bool operator==(const CircularBufferBase& other) const noexcept;

    bool operator!=(const CircularBufferBase& other) const noexcept;

    value_type pop_back();

    value_type pop_front();

    size_type size() const noexcept;

    size_type capacity() const noexcept;

    size_type max_size() const noexcept;

    bool empty() const noexcept;

    reference front();

    const_reference front() const;

    reference back();

    const_reference back() const;

    void reserve(size_type n);

    void resize(size_type n, const value_type& value = value_type());

    allocator_type get_allocator() const noexcept;

protected:
    explicit CircularBufferBase(const Alloc& allocator = Alloc());

    explicit CircularBufferBase(size_type size, const Alloc& allocator = Alloc());

    CircularBufferBase(size_type size, const_reference value, const Alloc& allocator = Alloc());

    template<typename LegacyInputIterator>
    requires std::input_iterator<LegacyInputIterator>
    CircularBufferBase(LegacyInputIterator i, LegacyInputIterator j, const Alloc& allocator = Alloc());

    CircularBufferBase(const std::initializer_list<value_type>& list, const Alloc& allocator = Alloc());

    CircularBufferBase(const CircularBufferBase& other);

    CircularBufferBase(CircularBufferBase&& other) noexcept;

    CircularBufferBase& operator=(const CircularBufferBase& other) noexcept;

    CircularBufferBase& operator=(CircularBufferBase&& other) noexcept;

    CircularBufferBase& operator=(const std::initializer_list<value_type>& list);

    pointer buff_start_;
    pointer buff_end_;
    pointer actual_start_;
    pointer actual_end_;

    [[no_unique_address]] allocator_type allocator_;
};


template<typename T, typename Alloc>
CircularBufferBase<T, Alloc>::CircularBufferBase(const Alloc& allocator)
        : allocator_(allocator),
          buff_start_(AllocTraits::allocate(allocator_, 1)),
          buff_end_(buff_start_ + 1),
          actual_start_(buff_start_),
          actual_end_(buff_start_) {}


template<typename T, typename Alloc>
CircularBufferBase<T, Alloc>::CircularBufferBase(size_type size, const Alloc& allocator)
        : allocator_(allocator),
          buff_start_(AllocTraits::allocate(allocator_, size + 1)),
          buff_end_(buff_start_ + size + 1),
          actual_start_(buff_start_),
          actual_end_(buff_start_) {}

template<typename T, typename Alloc>
CircularBufferBase<T, Alloc>::CircularBufferBase(size_type size, const_reference value, const Alloc& allocator)
        : allocator_(allocator),
          buff_start_(AllocTraits::allocate(allocator_, size + 1)),
          buff_end_(buff_start_ + size + 1),
          actual_start_(buff_start_),
          actual_end_(buff_end_ - 1) {
    size_type current = 0;
    try {
        for (; current < size; ++current) {
            AllocTraits::construct(allocator_, actual_start_ + current, value);
        }
    } catch (...) {
        for (size_type i = 0; i < current; ++i) {
            AllocTraits::destroy(allocator_, actual_start_ + i);
        }
        AllocTraits::deallocate(allocator_, buff_start_, size + 1);
        throw;
    }
}


template<typename T, typename Alloc>
template<typename LegacyInputIterator>
requires std::input_iterator<LegacyInputIterator>
CircularBufferBase<T, Alloc>::CircularBufferBase(LegacyInputIterator i, LegacyInputIterator j,
                                                 const Alloc& allocator)
        : allocator_(allocator),
          buff_start_(AllocTraits::allocate(allocator_, std::distance(i, j) + 1)),
          buff_end_(buff_start_ + std::distance(i, j) + 1),
          actual_start_(buff_start_),
          actual_end_(buff_end_ - 1) {
    try {
        my_uninitialized_copy(i, j, actual_start_, allocator_);
    } catch (...) {
        AllocTraits::deallocate(allocator_, buff_start_, std::distance(buff_start_, buff_end_));
        throw;
    }
}

template<typename T, typename Alloc>
CircularBufferBase<T, Alloc>::CircularBufferBase(const std::initializer_list<value_type>& list,
                                                 const Alloc& allocator)
        : allocator_(allocator),
          buff_start_(AllocTraits::allocate(allocator_, list.size() + 1)),
          buff_end_(buff_start_ + list.size() + 1),
          actual_start_(buff_start_),
          actual_end_(buff_end_ - 1) {
    try {
        my_uninitialized_copy(list.begin(), list.end(), actual_start_, allocator_);
    } catch (...) {
        AllocTraits::deallocate(allocator_, buff_start_, list.size() + 1);
        throw;
    }
}


template<typename T, typename Alloc>
CircularBufferBase<T, Alloc>::CircularBufferBase(const CircularBufferBase& other)
        : allocator_(AllocTraits::select_on_container_copy_construction(other.allocator_)),
          buff_start_(AllocTraits::allocate(allocator_, other.size() + 1)),
          buff_end_(buff_start_ + other.size() + 1),
          actual_start_(buff_start_),
          actual_end_(actual_start_ + other.size()) {
    try {
        my_uninitialized_copy(other.begin(), other.end(), buff_start_, allocator_);
    } catch (...) {
        AllocTraits::deallocate(allocator_, buff_start_, capacity() + 1);
        throw;
    }
}

template<typename T, typename Alloc>
CircularBufferBase<T, Alloc>::CircularBufferBase(CircularBufferBase&& other) noexcept
        : allocator_(std::move(other.allocator_)),
          buff_start_(other.buff_start_),
          buff_end_(other.buff_end_),
          actual_start_(other.actual_start_),
          actual_end_(other.actual_end_) {
    other.buff_start_ = other.buff_end_ = other.actual_start_ = other.actual_end_ = nullptr;
}

template<typename T, typename Alloc>
CircularBufferBase<T, Alloc>& CircularBufferBase<T, Alloc>::operator=(const CircularBufferBase& other) noexcept {
    if (this == &other) {
        return *this;
    }
    if constexpr (AllocTraits::propagate_on_container_copy_assignment::value) {
        allocator_type new_allocator = other;

        auto new_buff_start = AllocTraits::allocate(new_allocator, other.size() + 1);
        try {
            my_uninitialized_copy(other.begin(), other.end(), new_buff_start, new_allocator);
        } catch (...) {
            AllocTraits::deallocate(new_allocator, new_buff_start, other.size() + 1);
            throw;
        }

        clear();
        AllocTraits::deallocate(allocator_, buff_start_, capacity() + 1);

        allocator_ = std::move(new_allocator);
        buff_start_ = new_buff_start;
        buff_end_ = new_buff_start + other.size() + 1;
        actual_start_ = buff_start_;
        actual_end_ = buff_end_ - 1;

        return *this;
    }

    pointer new_buff_start = AllocTraits::allocate(allocator_, other.size() + 1);
    try {
        my_uninitialized_copy(other.begin(), other.end(), new_buff_start, allocator_);
    } catch (...) {
        AllocTraits::deallocate(allocator_, new_buff_start, other.size() + 1);
        throw;
    }

    clear();
    AllocTraits::deallocate(allocator_, buff_start_, std::distance(buff_start_, buff_end_));

    buff_start_ = new_buff_start;
    buff_end_ = new_buff_start + other.size() + 1;
    actual_start_ = buff_start_;
    actual_end_ = buff_end_ - 1;

    return *this;
}

template<typename T, typename Alloc>
CircularBufferBase<T, Alloc>& CircularBufferBase<T, Alloc>::operator=(CircularBufferBase&& other) noexcept {
    if (this == &other) {
        return *this;
    }
    if constexpr (AllocTraits::propagate_on_container_move_assignment::value) {
        clear();
        AllocTraits::deallocate(allocator_, buff_start_, std::distance(buff_start_, buff_end_));
        allocator_ = std::move(other.allocator_);
        buff_start_ = other.buff_start_;
        buff_end_ = other.buff_end_;
        actual_start_ = other.actual_start_;
        actual_end_ = other.actual_end_;

        other.buff_start_ = other.buff_end_ = other.actual_start_ = other.actual_end_ = nullptr;
        return *this;
    }

    pointer new_buff_start = AllocTraits::allocate(allocator_, other.capacity() + 1);
    try {
        my_uninitialized_move(other.begin(), other.end(), new_buff_start, allocator_);
    } catch (...) {
        AllocTraits::deallocate(allocator_, new_buff_start, other.capacity() + 1);
        throw;
    }

    clear();
    AllocTraits::deallocate(allocator_, buff_start_, std::distance(buff_start_, buff_end_));
    buff_start_ = new_buff_start;
    buff_end_ = new_buff_start + other.capacity() + 1;
    actual_start_ = buff_start_;
    actual_end_ = actual_start_ + other.size();

    return *this;
}

template<typename T, typename Alloc>
CircularBufferBase<T, Alloc>&
CircularBufferBase<T, Alloc>::operator=(const std::initializer_list<value_type>& list) {
    pointer new_arr = AllocTraits::allocate(allocator_, list.size() + 1);

    try {
        my_uninitialized_copy(list.begin(), list.end(), new_arr, allocator_);
    } catch (...) {
        AllocTraits::deallocate(allocator_, new_arr, list.size() + 1);
        throw;
    }

    clear();
    AllocTraits::deallocate(allocator_, buff_start_, capacity() + 1);

    buff_start_ = new_arr;
    buff_end_ = buff_start_ + list.size() + 1;
    actual_start_ = buff_start_;
    actual_end_ = buff_end_ - 1;

    return *this;
}

template<typename T, typename Alloc>
CircularBufferBase<T, Alloc>::value_type CircularBufferBase<T, Alloc>::pop_back() {
    if (empty()) {
        throw std::out_of_range("Trying to pop_back() from an empty buffer");
    }
    actual_end_ = (actual_end_ == buff_start_ ? buff_end_ - 1 : actual_end_ - 1);
    auto to_return = std::move(*actual_end_);
    AllocTraits::destroy(allocator_, actual_end_);
    return to_return;
}

template<typename T, typename Alloc>
CircularBufferBase<T, Alloc>::value_type CircularBufferBase<T, Alloc>::pop_front() {
    if (empty()) {
        throw std::out_of_range("Trying to pop_back() from an empty buffer");
    }
    auto to_return = std::move(*actual_start_);
    AllocTraits::destroy(allocator_, actual_start_);
    actual_start_ = (actual_start_ + 1 == buff_end_ ? buff_start_ : actual_start_ + 1);

    return to_return;
}

template<typename T, typename Alloc>
CircularBufferBase<T, Alloc>::size_type CircularBufferBase<T, Alloc>::size() const noexcept {
    return std::distance(cbegin(), cend());
}

template<typename T, typename Alloc>
CircularBufferBase<T, Alloc>::size_type CircularBufferBase<T, Alloc>::capacity() const noexcept {
    return std::distance(buff_start_, buff_end_) - 1;
}

template<typename T, typename Alloc>
CircularBufferBase<T, Alloc>::reference CircularBufferBase<T, Alloc>::operator[](size_type i) {
    return *(begin() + i);
}

template<typename T, typename Alloc>
CircularBufferBase<T, Alloc>::const_reference CircularBufferBase<T, Alloc>::operator[](size_type i) const {
    return *(cbegin() + i);
}

template<typename T, typename Alloc>
CircularBufferBase<T, Alloc>::iterator CircularBufferBase<T, Alloc>::begin() noexcept {
    return CircularBufferBase::iterator(actual_start_, buff_start_, buff_end_, actual_start_, actual_end_);
}

template<typename T, typename Alloc>
CircularBufferBase<T, Alloc>::iterator CircularBufferBase<T, Alloc>::end() noexcept {
    return CircularBufferBase::iterator(actual_end_, buff_start_, buff_end_, actual_start_, actual_end_);
}

template<typename T, typename Alloc>
CircularBufferBase<T, Alloc>::const_iterator CircularBufferBase<T, Alloc>::begin() const noexcept {
    return CircularBufferBase::const_iterator(actual_start_, buff_start_, buff_end_, actual_start_, actual_end_);
}

template<typename T, typename Alloc>
CircularBufferBase<T, Alloc>::const_iterator CircularBufferBase<T, Alloc>::end() const noexcept {
    return CircularBufferBase::const_iterator(actual_end_, buff_start_, buff_end_, actual_start_, actual_end_);
}

template<typename T, typename Alloc>
CircularBufferBase<T, Alloc>::const_iterator CircularBufferBase<T, Alloc>::cbegin() const noexcept {
    return CircularBufferBase::const_iterator(actual_start_, buff_start_, buff_end_, actual_start_, actual_end_);
}

template<typename T, typename Alloc>
CircularBufferBase<T, Alloc>::const_iterator CircularBufferBase<T, Alloc>::cend() const noexcept {
    return CircularBufferBase::const_iterator(actual_end_, buff_start_, buff_end_, actual_start_, actual_end_);
}

template<typename T, typename Alloc>
CircularBufferBase<T, Alloc>::reverse_iterator CircularBufferBase<T, Alloc>::rbegin() noexcept {
    return reverse_iterator(iterator(actual_end_, buff_start_, buff_end_, actual_start_, actual_end_));
}

template<typename T, typename Alloc>
CircularBufferBase<T, Alloc>::reverse_iterator CircularBufferBase<T, Alloc>::rend() noexcept {
    return reverse_iterator(iterator(actual_start_, buff_start_, buff_end_, actual_start_, actual_end_));
}

template<typename T, typename Alloc>
CircularBufferBase<T, Alloc>::const_reverse_iterator CircularBufferBase<T, Alloc>::rbegin() const noexcept {
    return const_reverse_iterator(iterator(actual_end_, buff_start_, buff_end_, actual_start_, actual_end_));
}

template<typename T, typename Alloc>
CircularBufferBase<T, Alloc>::const_reverse_iterator CircularBufferBase<T, Alloc>::rend() const noexcept {
    return const_reverse_iterator(iterator(actual_start_, buff_start_, buff_end_, actual_start_, actual_end_));
}

template<typename T, typename Alloc>
CircularBufferBase<T, Alloc>::const_reverse_iterator CircularBufferBase<T, Alloc>::crbegin() const noexcept {
    return const_reverse_iterator(const_iterator(actual_end_, buff_start_, buff_end_, actual_start_, actual_end_));
}


template<typename T, typename Alloc>
CircularBufferBase<T, Alloc>::const_reverse_iterator CircularBufferBase<T, Alloc>::crend() const noexcept {
    return const_reverse_iterator(const_iterator(actual_start_, buff_start_, buff_end_, actual_start_, actual_end_));
}


template<typename T, typename Alloc>
bool CircularBufferBase<T, Alloc>::operator==(const CircularBufferBase& other) const noexcept {
    if (this == &other) {
        return true;
    }
    return std::equal(cbegin(), cend(), other.cbegin(), other.cend());
}

template<typename T, typename Alloc>
void CircularBufferBase<T, Alloc>::swap(CircularBufferBase& other) {
    if (this == &other) {
        return;
    }
    if constexpr (AllocTraits::propagate_on_container_swap::value) {
        std::swap(this->allocator_, other.allocator_);
        std::swap(buff_start_, other.buff_start_);
        std::swap(buff_end_, other.buff_end_);
        std::swap(actual_start_, other.actual_start_);
        std::swap(actual_end_, other.actual_end_);
        return;
    }
    const size_type this_old_size = this->size();
    const size_type this_old_capacity = this->capacity();
    const size_type other_old_size = other.size();
    const size_type other_old_capacity = other.capacity();

    pointer new_this_buff_start = AllocTraits::allocate(allocator_, other_old_capacity + 1);
    pointer new_other_buff_start;
    try {
        new_other_buff_start = AllocTraits::allocate(other.allocator_, this_old_size + 1);
    } catch (...) {
        AllocTraits::deallocate(allocator_, new_this_buff_start, other_old_capacity + 1);
        throw;
    }

    try {
        my_uninitialized_move(this->begin(), this->end(), new_other_buff_start, other.allocator_);
        my_uninitialized_move(other.begin(), other.end(), new_this_buff_start, this->allocator_);
    } catch (...) {
        AllocTraits::deallocate(allocator_, new_this_buff_start, other_old_capacity + 1);
        AllocTraits::deallocate(other.allocator_, new_other_buff_start, this_old_capacity + 1);
        throw;
    }


    clear();
    other.clear();
    AllocTraits::deallocate(allocator_, this->buff_start_, this_old_capacity + 1);
    AllocTraits::deallocate(other.allocator_, other.buff_start_, other_old_capacity + 1);

    this->buff_start_ = new_this_buff_start;
    this->buff_end_ = new_this_buff_start + other_old_capacity + 1;
    this->actual_start_ = this->buff_start_;
    this->actual_end_ = this->actual_start_ + other_old_size;

    other.buff_start_ = new_other_buff_start;
    other.buff_end_ = new_other_buff_start + this_old_capacity + 1;
    other.actual_start_ = other.buff_start_;
    other.actual_end_ = other.actual_start_ + this_old_size;

}

template<typename T, typename Alloc>
bool CircularBufferBase<T, Alloc>::operator!=(const CircularBufferBase& other) const noexcept {
    return !this->operator==(other);
}

template<typename T, typename Alloc>
bool CircularBufferBase<T, Alloc>::empty() const noexcept {
    return size() == 0;
}

template<typename T, typename Alloc>
CircularBufferBase<T, Alloc>::size_type CircularBufferBase<T, Alloc>::max_size() const noexcept {
    return std::min(AllocTraits::max_size(*this),
                    std::numeric_limits<std::ranges::__detail::__max_size_type>::max() / sizeof(size_type));
}

template<typename T, typename Alloc>
void CircularBufferBase<T, Alloc>::reserve(CircularBufferBase<T, Alloc>::size_type n) {
    if (capacity() >= n) {
        return;
    }
    auto new_buff_start = AllocTraits::allocate(allocator_, n + 1);
    try {
        my_uninitialized_move(begin(), end(), new_buff_start, allocator_);
    } catch (...) {
        AllocTraits::deallocate(allocator_, new_buff_start, n + 1);
        throw;
    }
    auto old_size = size();
    clear();
    AllocTraits::deallocate(allocator_, buff_start_, capacity() + 1);

    buff_start_ = new_buff_start;
    buff_end_ = buff_start_ + n + 1;
    actual_start_ = buff_start_;
    actual_end_ = actual_start_ + old_size;
}

template<typename T, typename Alloc>
void CircularBufferBase<T, Alloc>::resize(size_type n, const value_type& value) {
    if (n == size()) {
        return;
    }
    if (n > capacity()) {
        reserve(n);
    }
    if (n > size()) {
        size_type number_of_new_values = n - size();
        size_type i = 0;
        try {
            for (; i < number_of_new_values; ++i) {
                AllocTraits::construct(allocator_, actual_end_, value);
                actual_end_ = (actual_end_ + 1 == buff_end_ ? buff_start_ : actual_end_ + 1);
            }
        } catch (...) {
            for (size_type j = 0; j < i; ++j) {
                actual_end_ = (actual_end_ == buff_start_ ? buff_end_ - 1 : actual_end_ - 1);
                AllocTraits::destroy(allocator_, actual_end_ + j);
            }
            throw;
        }
        return;
    }
    size_type number_of_values_to_remove = size() - n;
    for (size_type i = 0; i < number_of_values_to_remove; ++i) {
        actual_end_ = (actual_end_ == buff_start_ ? buff_end_ - 1 : actual_end_ - 1);
        AllocTraits::destroy(allocator_, actual_end_);
    }
}

template<typename T, typename Alloc>
CircularBufferBase<T, Alloc>::iterator CircularBufferBase<T, Alloc>::erase(CircularBufferBase::const_iterator q) {
    if (std::addressof(*q) < buff_start_ || std::addressof(*q) >= buff_end_) {
        throw std::out_of_range("Iterator is out of bounds");
    }
    size_type index = q - cbegin();
    if (index >= size()) {
        throw std::out_of_range("Iterator is out of bounds");
    }
    for (auto it = begin() + index; index < size() - 1; ++index, ++it) {
        *it = std::move_if_noexcept(*(it + 1));
    }
    actual_end_ = (actual_end_ == buff_start_ ? buff_end_ - 1 : actual_end_ - 1);
    AllocTraits::destroy(allocator_, actual_end_);

    return begin() + index;
}

template<typename T, typename Alloc>
CircularBufferBase<T, Alloc>::iterator CircularBufferBase<T, Alloc>::erase(CircularBufferBase::const_iterator q1,
                                                                           CircularBufferBase::const_iterator q2) {
    if (std::addressof(*q1) < buff_start_ || std::addressof(*q1) >= buff_end_ ||
        std::addressof(*q2) < buff_start_ || std::addressof(*q2) >= buff_end_) {
        throw std::out_of_range("Iterator is out of bounds");
    }
    const size_type index_start = q1 - cbegin();
    const size_type index_end = (q2 - cbegin()) - 1;
    const size_type number_of_elements = index_end - index_start + 1;
    if (index_start >= size() || index_end >= size()) {
        throw std::out_of_range("Iterator is out of bounds");
    }

    for (auto it = q1; it != q2; ++it) {
        AllocTraits::destroy(allocator_, std::addressof(*it));
    }
    for (auto it = begin() + index_end + 1; it != end(); ++it) {
        *(it - number_of_elements) = std::move_if_noexcept(*it);
    }

    actual_end_ = std::addressof(*(end() - number_of_elements));
    return begin() + index_start;
}

template<typename T, typename Alloc>
void CircularBufferBase<T, Alloc>::clear() noexcept {
    for (auto it = cbegin(); it != cend(); ++it) {
        AllocTraits::destroy(allocator_, std::addressof(*it));
    }
    actual_start_ = buff_start_;
    actual_end_ = buff_start_;
}

template<typename T, typename Alloc>
void CircularBufferBase<T, Alloc>::assign(CircularBufferBase::size_type n, const_reference value) {
    pointer new_arr = AllocTraits::allocate(allocator_, n + 1);
    try {
        my_uninitialized_copy(n, value, new_arr, allocator_);
    } catch (...) {
        AllocTraits::deallocate(allocator_, new_arr, n + 1);
        throw;
    }
    clear();
    AllocTraits::deallocate(allocator_, buff_start_, capacity() + 1);

    buff_start_ = new_arr;
    buff_end_ = new_arr + n + 1;
    actual_start_ = buff_start_;
    actual_end_ = buff_end_ - 1;
}

template<typename T, typename Alloc>
template<typename LegacyInputIterator>
requires std::input_iterator<LegacyInputIterator>
void CircularBufferBase<T, Alloc>::assign(LegacyInputIterator i, LegacyInputIterator j) {
    size_type new_size = std::distance(i, j);
    pointer new_arr = AllocTraits::allocate(allocator_, new_size + 1);
    try {
        my_uninitialized_copy(i, j, new_arr, allocator_);
    } catch (...) {
        AllocTraits::deallocate(allocator_, new_arr, new_size + 1);
        throw;
    }

    clear();
    AllocTraits::deallocate(allocator_, buff_start_, capacity() + 1);

    buff_start_ = new_arr;
    buff_end_ = new_arr + new_size + 1;
    actual_start_ = buff_start_;
    actual_end_ = buff_end_ - 1;
}

template<typename T, typename Alloc>
void CircularBufferBase<T, Alloc>::assign(const std::initializer_list<value_type>& il) {
    assign(il.begin(), il.end());
}

template<typename T, typename Alloc>
CircularBufferBase<T, Alloc>::reference CircularBufferBase<T, Alloc>::front() {
    if (empty()) {
        throw std::out_of_range("Trying to get data from empty buffer");
    }
    return *actual_start_;
}

template<typename T, typename Alloc>
CircularBufferBase<T, Alloc>::const_reference CircularBufferBase<T, Alloc>::front() const {
    if (empty()) {
        throw std::out_of_range("Trying to get data from empty buffer");
    }
    return *actual_start_;
}

template<typename T, typename Alloc>
CircularBufferBase<T, Alloc>::reference CircularBufferBase<T, Alloc>::back() {
    if (empty()) {
        throw std::out_of_range("Trying to get data from empty buffer");
    }
    return *(actual_end_ == buff_start_ ? buff_end_ - 1 : actual_end_ - 1);
}

template<typename T, typename Alloc>
CircularBufferBase<T, Alloc>::const_reference CircularBufferBase<T, Alloc>::back() const {
    if (empty()) {
        throw std::out_of_range("Trying to get data from empty buffer");
    }
    return *(actual_end_ == buff_start_ ? buff_end_ - 1 : actual_end_ - 1);
}

template<typename T, typename Alloc>
CircularBufferBase<T, Alloc>::allocator_type CircularBufferBase<T, Alloc>::get_allocator() const noexcept {
    return allocator_;
}
