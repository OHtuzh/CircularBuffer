#pragma once

#include "Iterator.hpp"
#include "uninitialized_copy_modified.hpp"


#define USING_FIELDS \
    using typename CircularBufferTraits<T, Alloc>::allocator_type; \
    using typename CircularBufferTraits<T, Alloc>::AllocTraits; \
    using typename CircularBufferTraits<T, Alloc>::iterator; \
    using typename CircularBufferTraits<T, Alloc>::const_iterator; \
    using typename CircularBufferTraits<T, Alloc>::reverse_iterator; \
    using typename CircularBufferTraits<T, Alloc>::const_reverse_iterator; \
    using typename CircularBufferTraits<T, Alloc>::value_type; \
    using typename CircularBufferTraits<T, Alloc>::reference; \
    using typename CircularBufferTraits<T, Alloc>::pointer; \
    using typename CircularBufferTraits<T, Alloc>::const_reference; \
    using typename CircularBufferTraits<T, Alloc>::difference_type; \
    using typename CircularBufferTraits<T, Alloc>::size_type; \
    using CircularBufferTraits<T, Alloc>::begin; \
    using CircularBufferTraits<T, Alloc>::end; \
    using CircularBufferTraits<T, Alloc>::rbegin; \
    using CircularBufferTraits<T, Alloc>::rend; \
    using CircularBufferTraits<T, Alloc>::cbegin; \
    using CircularBufferTraits<T, Alloc>::cend; \
    using CircularBufferTraits<T, Alloc>::crbegin; \
    using CircularBufferTraits<T, Alloc>::swap; \
    using CircularBufferTraits<T, Alloc>::size; \
    using CircularBufferTraits<T, Alloc>::capacity; \
    using CircularBufferTraits<T, Alloc>::max_size; \
    using CircularBufferTraits<T, Alloc>::empty; \
    using CircularBufferTraits<T, Alloc>::reserve; \
    using CircularBufferTraits<T, Alloc>::resize; \
    using CircularBufferTraits<T, Alloc>::erase; \
    using CircularBufferTraits<T, Alloc>::clear; \
    using CircularBufferTraits<T, Alloc>::assign;


template<typename T, typename Alloc = std::allocator<T>>
class CircularBufferTraits : protected std::allocator_traits<Alloc>::template rebind_alloc<T> {
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


    explicit CircularBufferTraits(const Alloc& allocator = Alloc());

    explicit CircularBufferTraits(size_type size, const Alloc& allocator = Alloc());

    CircularBufferTraits(size_type size, const_reference value, const Alloc& allocator = Alloc());

    template<typename LegacyInputIterator>
    CircularBufferTraits(LegacyInputIterator i, LegacyInputIterator j, const Alloc& allocator = Alloc());

    CircularBufferTraits(const std::initializer_list<value_type>& list, const Alloc& allocator = Alloc());

    CircularBufferTraits(const CircularBufferTraits& other);

    CircularBufferTraits(CircularBufferTraits&& other) noexcept;

    CircularBufferTraits& operator=(const CircularBufferTraits& other) noexcept;

    CircularBufferTraits& operator=(CircularBufferTraits&& other) noexcept;

    CircularBufferTraits& operator=(const std::initializer_list<value_type>& list);

    void swap(CircularBufferTraits& other);

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
    void assign(LegacyInputIterator i, LegacyInputIterator j);

    void assign(const std::initializer_list<value_type>& il);

    bool operator==(const CircularBufferTraits& other) const noexcept;

    bool operator!=(const CircularBufferTraits& other) const noexcept;


    size_type size() const noexcept;

    size_type capacity() const noexcept;

    size_type max_size() const noexcept;

    bool empty() const noexcept;


    void reserve(size_type n);

    void resize(size_type n, const value_type& value = value_type());

protected:
    pointer buff_start_;
    pointer buff_end_;
    pointer actual_start_;
    pointer actual_end_;
};


template<typename T, typename Alloc>
CircularBufferTraits<T, Alloc>::CircularBufferTraits(const Alloc& allocator)
        :
        allocator_type(allocator),
        buff_start_(AllocTraits::allocate(*this, 1)),
        buff_end_(buff_start_ + 1),
        actual_start_(buff_start_),
        actual_end_(buff_start_) {}


template<typename T, typename Alloc>
CircularBufferTraits<T, Alloc>::CircularBufferTraits(size_type size, const Alloc& allocator) :
        allocator_type(allocator),
        buff_start_(AllocTraits::allocate(*this, size + 1)),
        buff_end_(buff_start_ + size + 1),
        actual_start_(buff_start_),
        actual_end_(buff_start_ + size) {}

template<typename T, typename Alloc>
CircularBufferTraits<T, Alloc>::CircularBufferTraits(size_type size, const_reference value, const Alloc& allocator)
        :
        allocator_type(allocator),
        buff_start_(AllocTraits::allocate(*this, size + 1)),
        buff_end_(buff_start_ + size + 1),
        actual_start_(buff_start_),
        actual_end_(buff_end_) {
    size_type current = 0;
    try {
        for (; current < size; ++current) {
            AllocTraits::construct(*this, actual_start_ + current, value);
        }
    } catch (...) {
        for (size_type i = 0; i < current; ++i) {
            AllocTraits::destroy(*this, actual_start_ + i);
        }
        AllocTraits::deallocate(*this, buff_start_, size + 1);
        throw;
    }
}


template<typename T, typename Alloc>
template<typename LegacyInputIterator>
CircularBufferTraits<T, Alloc>::CircularBufferTraits(LegacyInputIterator i, LegacyInputIterator j,
                                                     const Alloc& allocator)
        :
        allocator_type(allocator),
        buff_start_(AllocTraits::allocate(*this, std::distance(i, j) + 1)),
        buff_end_(buff_start_ + std::distance(i, j) + 1),
        actual_start_(buff_start_),
        actual_end_(buff_end_ + 1) {
    try {
        my_uninitialized_copy(i, j, actual_start_, *this);
    } catch (...) {
        AllocTraits::deallocate(*this, buff_start_, std::distance(buff_start_, buff_end_));
        throw;
    }
}

template<typename T, typename Alloc>
CircularBufferTraits<T, Alloc>::CircularBufferTraits(const std::initializer_list<value_type>& list,
                                                     const Alloc& allocator)
        :
        allocator_type(allocator),
        buff_start_(AllocTraits::allocate(*this, list.size() + 1)),
        buff_end_(buff_start_ + list.size() + 1),
        actual_start_(buff_start_),
        actual_end_(buff_end_ - 1) {
    try {
        my_uninitialized_copy(list.begin(), list.end(), actual_start_, *this);
    } catch (...) {
        AllocTraits::deallocate(*this, buff_start_, list.size() + 1);
        throw;
    }
}


template<typename T, typename Alloc>
CircularBufferTraits<T, Alloc>::CircularBufferTraits(const CircularBufferTraits& other)
        :
        allocator_type(AllocTraits::select_on_container_copy_construction(other)),
        buff_start_(AllocTraits::allocate(*this, other.size() + 1)),
        buff_end_(buff_start_ + other.size() + 1),
        actual_start_(buff_start_),
        actual_end_(actual_start_ + other.size()) {
    try {
        my_uninitialized_copy(other.begin(), other.end(), buff_start_, *this);
    } catch (...) {
        AllocTraits::deallocate(*this, buff_start_, capacity() + 1);
    }
}

template<typename T, typename Alloc>
CircularBufferTraits<T, Alloc>::CircularBufferTraits(CircularBufferTraits&& other) noexcept
        :
        allocator_type(std::move(static_cast<allocator_type>(other))),
        buff_start_(other.buff_start_),
        buff_end_(other.buff_end_),
        actual_start_(other.actual_start_),
        actual_end_(other.actual_end_) {
    other.buff_start_ = other.buff_end_ = other.actual_start_ = other.actual_end_ = nullptr;
}

template<typename T, typename Alloc>
CircularBufferTraits<T, Alloc>& CircularBufferTraits<T, Alloc>::operator=(const CircularBufferTraits& other) noexcept {
    if (this == &other) {
        return *this;
    }
    if constexpr (AllocTraits::propagate_on_container_copy_assignment::value) {
        allocator_type allocator = other;

        auto new_buff_start = AllocTraits::allocate(allocator, other.size() + 1);
        try {
            my_uninitialized_copy(other.begin(), other.end(), new_buff_start, allocator);
        } catch (...) {
            AllocTraits::deallocate(allocator, new_buff_start, other.size() + 1);
            throw;
        }

        clear();
        AllocTraits::deallocate(*this, buff_start_, capacity() + 1);

        static_cast<allocator_type&>(*this) = std::move(allocator);
        buff_start_ = new_buff_start;
        buff_end_ = new_buff_start + other.size() + 1;
        actual_start_ = buff_start_;
        actual_end_ = buff_end_ - 1;

        return *this;
    }

    pointer new_buff_start = AllocTraits::allocate(*this, other.size() + 1);
    try {
        my_uninitialized_copy(other.begin(), other.end(), new_buff_start, *this);
    } catch (...) {
        AllocTraits::deallocate(*this, new_buff_start, other.size() + 1);
        throw;
    }

    clear();
    AllocTraits::deallocate(*this, buff_start_, std::distance(buff_start_, buff_end_));

    buff_start_ = new_buff_start;
    buff_end_ = new_buff_start + other.size() + 1;
    actual_start_ = buff_start_;
    actual_end_ = buff_end_ - 1;

    return *this;
}

template<typename T, typename Alloc>
CircularBufferTraits<T, Alloc>& CircularBufferTraits<T, Alloc>::operator=(CircularBufferTraits&& other) noexcept {
    if (this == &other) {
        return *this;
    }
    if constexpr (AllocTraits::propagate_on_container_move_assignment::value) {
        clear();
        AllocTraits::deallocate(*this, buff_start_, std::distance(buff_start_, buff_end_));
        static_cast<allocator_type&>(*this) = std::move(static_cast<allocator_type&>(other));
        buff_start_ = other.buff_start_;
        buff_end_ = other.buff_end_;
        actual_start_ = other.actual_start_;
        actual_end_ = other.actual_end_;

        other.buff_start_ = other.buff_end_ = other.actual_start_ = other.actual_end_ = nullptr;
        return *this;
    }

    pointer new_buff_start = AllocTraits::allocate(*this, other.capacity() + 1);
    try {
        my_uninitialized_move(other.begin(), other.end(), new_buff_start, *this);
    } catch (...) {
        AllocTraits::deallocate(*this, new_buff_start, other.capacity() + 1);
        throw;
    }

    clear();
    AllocTraits::deallocate(*this, buff_start_, std::distance(buff_start_, buff_end_));
    buff_start_ = new_buff_start;
    buff_end_ = new_buff_start + other.capacity() + 1;
    actual_start_ = buff_start_;
    actual_end_ = actual_start_ + other.size();

    // Perhaps it is necessary to deallocate memory that other takes, but I am not sure
    return *this;
}

template<typename T, typename Alloc>
CircularBufferTraits<T, Alloc>&
CircularBufferTraits<T, Alloc>::operator=(const std::initializer_list<value_type>& list) {
    pointer new_arr = AllocTraits::allocate(*this, list.size() + 1);

    try {
        my_uninitialized_copy(list.begin(), list.end(), new_arr, *this);
    } catch (...) {
        AllocTraits::deallocate(*this, new_arr, list.size() + 1);
        throw;
    }

    clear();
    AllocTraits::deallocate(*this, buff_start_, capacity() + 1);

    buff_start_ = new_arr;
    buff_end_ = buff_start_ + list.size() + 1;
    actual_start_ = buff_start_;
    actual_end_ = buff_end_ - 1;

    return *this;
}

template<typename T, typename Alloc>
CircularBufferTraits<T, Alloc>::size_type CircularBufferTraits<T, Alloc>::size() const noexcept {
    return std::distance(cbegin(), cend());
}

template<typename T, typename Alloc>
CircularBufferTraits<T, Alloc>::size_type CircularBufferTraits<T, Alloc>::capacity() const noexcept {
    return std::distance(buff_start_, buff_end_) - 1;
}

template<typename T, typename Alloc>
CircularBufferTraits<T, Alloc>::reference CircularBufferTraits<T, Alloc>::operator[](size_type i) {
    return *(begin() + i);
}

template<typename T, typename Alloc>
CircularBufferTraits<T, Alloc>::const_reference CircularBufferTraits<T, Alloc>::operator[](size_type i) const {
    return *(cbegin() + i);
}

template<typename T, typename Alloc>
CircularBufferTraits<T, Alloc>::iterator CircularBufferTraits<T, Alloc>::begin() noexcept {
    return CircularBufferTraits::iterator(actual_start_, buff_start_, buff_end_, actual_start_, actual_end_);
}

template<typename T, typename Alloc>
CircularBufferTraits<T, Alloc>::iterator CircularBufferTraits<T, Alloc>::end() noexcept {
    return CircularBufferTraits::iterator(actual_end_, buff_start_, buff_end_, actual_start_, actual_end_);
}

template<typename T, typename Alloc>
CircularBufferTraits<T, Alloc>::const_iterator CircularBufferTraits<T, Alloc>::begin() const noexcept {
    return CircularBufferTraits::const_iterator(actual_start_, buff_start_, buff_end_, actual_start_, actual_end_);
}

template<typename T, typename Alloc>
CircularBufferTraits<T, Alloc>::const_iterator CircularBufferTraits<T, Alloc>::end() const noexcept {
    return CircularBufferTraits::const_iterator(actual_end_, buff_start_, buff_end_, actual_start_, actual_end_);
}

template<typename T, typename Alloc>
CircularBufferTraits<T, Alloc>::const_iterator CircularBufferTraits<T, Alloc>::cbegin() const noexcept {
    return CircularBufferTraits::const_iterator(actual_start_, buff_start_, buff_end_, actual_start_, actual_end_);
}

template<typename T, typename Alloc>
CircularBufferTraits<T, Alloc>::const_iterator CircularBufferTraits<T, Alloc>::cend() const noexcept {
    return CircularBufferTraits::const_iterator(actual_end_, buff_start_, buff_end_, actual_start_, actual_end_);
}

template<typename T, typename Alloc>
CircularBufferTraits<T, Alloc>::reverse_iterator CircularBufferTraits<T, Alloc>::rbegin() noexcept {
    return reverse_iterator(iterator(actual_end_, buff_start_, buff_end_, actual_start_, actual_end_));
}

template<typename T, typename Alloc>
CircularBufferTraits<T, Alloc>::reverse_iterator CircularBufferTraits<T, Alloc>::rend() noexcept {
    return reverse_iterator(iterator(actual_start_, buff_start_, buff_end_, actual_start_, actual_end_));
}

template<typename T, typename Alloc>
CircularBufferTraits<T, Alloc>::const_reverse_iterator CircularBufferTraits<T, Alloc>::rbegin() const noexcept {
    return const_reverse_iterator(iterator(actual_end_, buff_start_, buff_end_, actual_start_, actual_end_));
}

template<typename T, typename Alloc>
CircularBufferTraits<T, Alloc>::const_reverse_iterator CircularBufferTraits<T, Alloc>::rend() const noexcept {
    return const_reverse_iterator(iterator(actual_start_, buff_start_, buff_end_, actual_start_, actual_end_));
}

template<typename T, typename Alloc>
CircularBufferTraits<T, Alloc>::const_reverse_iterator CircularBufferTraits<T, Alloc>::crbegin() const noexcept {
    return const_reverse_iterator(const_iterator(actual_end_, buff_start_, buff_end_, actual_start_, actual_end_));
}


template<typename T, typename Alloc>
CircularBufferTraits<T, Alloc>::const_reverse_iterator CircularBufferTraits<T, Alloc>::crend() const noexcept {
    return const_reverse_iterator(const_iterator(actual_start_, buff_start_, buff_end_, actual_start_, actual_end_));
}


template<typename T, typename Alloc>
bool CircularBufferTraits<T, Alloc>::operator==(const CircularBufferTraits& other) const noexcept {
    if (this == &other) {
        return true;
    }
    return std::equal(cbegin(), cend(), other.cbegin(), other.cend());
}

template<typename T, typename Alloc>
void CircularBufferTraits<T, Alloc>::swap(CircularBufferTraits& other) {
    if (this == &other) {
        return;
    }
    if constexpr (AllocTraits::propagate_on_container_swap::value) {
        std::swap(static_cast<allocator_type&>(*this), static_cast<allocator_type&>(other));
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

    pointer new_this_buff_start = AllocTraits::allocate(*this, other_old_capacity + 1);
    pointer new_other_buff_start;
    try {
        new_other_buff_start = AllocTraits::allocate(other, this_old_size + 1);
    } catch (...) {
        AllocTraits::deallocate(*this, new_this_buff_start, other_old_capacity + 1);
        throw;
    }

    try {
        my_uninitialized_move(this->begin(), this->end(), new_other_buff_start, other);
        my_uninitialized_move(other.begin(), other.end(), new_this_buff_start, *this);
    } catch (...) {
        AllocTraits::deallocate(*this, new_this_buff_start, other_old_capacity + 1);
        AllocTraits::deallocate(other, new_other_buff_start, this_old_capacity + 1);
        throw;
    }


    clear();
    other.clear();
    AllocTraits::deallocate(*this, this->buff_start_, this_old_capacity + 1);
    AllocTraits::deallocate(other, other.buff_start_, other_old_capacity + 1);

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
bool CircularBufferTraits<T, Alloc>::operator!=(const CircularBufferTraits& other) const noexcept {
    return !this->operator==(other);
}

template<typename T, typename Alloc>
bool CircularBufferTraits<T, Alloc>::empty() const noexcept {
    return size() == 0;
}

template<typename T, typename Alloc>
CircularBufferTraits<T, Alloc>::size_type CircularBufferTraits<T, Alloc>::max_size() const noexcept {
    return 1232123123;
}

template<typename T, typename Alloc>
void CircularBufferTraits<T, Alloc>::reserve(CircularBufferTraits<T, Alloc>::size_type n) {
    if (capacity() >= n) {
        return;
    }
    auto new_buff_start = AllocTraits::allocate(*this, n + 1);
    try {
        my_uninitialized_move(begin(), end(), new_buff_start, *this);
    } catch (...) {
        AllocTraits::deallocate(*this, new_buff_start, n + 1);
        throw;
    }
    auto old_size = size();
    clear();
    AllocTraits::deallocate(*this, buff_start_, capacity() + 1);

    buff_start_ = new_buff_start;
    buff_end_ = buff_start_ + n + 1;
    actual_start_ = buff_start_;
    actual_end_ = actual_start_ + old_size;
}

template<typename T, typename Alloc>
void CircularBufferTraits<T, Alloc>::resize(size_type n, const value_type& value) {
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
                AllocTraits::construct(*this, actual_end_, value);
                actual_end_ = (actual_end_ + 1 == buff_end_ ? buff_start_ : actual_end_ + 1);
            }
        } catch (...) {
            for (size_type j = 0; j < i; ++j) {
                actual_end_ = (actual_end_ == buff_start_ ? buff_end_ - 1 : actual_end_ - 1);
                AllocTraits::destroy(*this, actual_end_ + j);
            }
            throw;
        }
        return;
    }
    size_type number_of_values_to_remove = size() - n;
    for (size_type i = 0; i < number_of_values_to_remove; ++i) {
        actual_end_ = (actual_end_ == buff_start_ ? buff_end_ - 1 : actual_end_ - 1);
        AllocTraits::destroy(*this, actual_end_);
    }
}

template<typename T, typename Alloc>
CircularBufferTraits<T, Alloc>::iterator CircularBufferTraits<T, Alloc>::erase(CircularBufferTraits::const_iterator q) {
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
    AllocTraits::destroy(*this, actual_end_);

    return begin() + index;
}

template<typename T, typename Alloc>
CircularBufferTraits<T, Alloc>::iterator CircularBufferTraits<T, Alloc>::erase(CircularBufferTraits::const_iterator q1,
                                                                               CircularBufferTraits::const_iterator q2) {
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
        AllocTraits::destroy(*this, std::addressof(*it));
    }
    for (auto it = begin() + index_end + 1; it != end(); ++it) {
        *(it - number_of_elements) = std::move_if_noexcept(*it);
    }

    return begin() + index_start;
}

template<typename T, typename Alloc>
void CircularBufferTraits<T, Alloc>::clear() noexcept {
    for (auto it = cbegin(); it != cend(); ++it) {
        AllocTraits::destroy(*this, std::addressof(*it));
    }
    actual_start_ = buff_start_;
    actual_end_ = buff_start_;
}

template<typename T, typename Alloc>
void CircularBufferTraits<T, Alloc>::assign(CircularBufferTraits::size_type n, const_reference value) {
    pointer new_arr = AllocTraits::allocate(*this, n + 1);
    try {
        my_uninitialized_copy(n, value, new_arr, *this);
    } catch (...) {
        AllocTraits ::deallocate(*this, new_arr, n + 1);
        throw;
    }
    clear();
    AllocTraits::deallocate(*this, buff_start_, capacity() + 1);

    buff_start_ = new_arr;
    buff_end_ = new_arr + n + 1;
    actual_start_ = buff_start_;
    actual_end_ = buff_end_ - 1;
}

template<typename T, typename Alloc>
template<typename LegacyInputIterator>
void CircularBufferTraits<T, Alloc>::assign(LegacyInputIterator i, LegacyInputIterator j) {
    size_type new_size = std::distance(i, j);
    pointer new_arr = AllocTraits ::allocate(*this, new_size + 1);
    try {
        my_uninitialized_copy(i, j, new_arr, *this);
    } catch (...) {
        AllocTraits ::deallocate(*this, new_arr, new_size + 1);
        throw;
    }

    clear();
    AllocTraits::deallocate(*this, buff_start_, capacity() + 1);

    buff_start_ = new_arr;
    buff_end_ = new_arr + new_size + 1;
    actual_start_ = buff_start_;
    actual_end_ = buff_end_ - 1;
}

template<typename T, typename Alloc>
void CircularBufferTraits<T, Alloc>::assign(const std::initializer_list<value_type>& il) {
    assign(il.begin(), il.end());
}
