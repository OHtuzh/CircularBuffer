#pragma once

#include "Iterator.hpp"
#include "uninitialized_copy_modified.hpp"


/*TODO
 * reserve()
 * resize()
 * insert()
 * erase()
 */

#define USING_FIELDS \
    using typename CircularBufferTraits<T, Alloc>::allocator_type; \
    using typename CircularBufferTraits<T, Alloc>::AllocTraits; \
    using typename CircularBufferTraits<T, Alloc>::iterator; \
    using typename CircularBufferTraits<T, Alloc>::const_iterator; \
    using typename CircularBufferTraits<T, Alloc>::reverse_iterator; \
    using typename CircularBufferTraits<T, Alloc>::const_reverse_iterator; \
    using typename CircularBufferTraits<T, Alloc>::value_type; \
    using typename CircularBufferTraits<T, Alloc>::reference; \
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
    using CircularBufferTraits<T, Alloc>::reserve;
//    using CircularBufferTraits<T, Alloc>::resize; \
//    using CircularBufferTraits<T, Alloc>::erase; \
//    using CircularBufferTraits<T, Alloc>::insert;


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

    using difference_type = iterator::difference_type;
    using size_type = std::size_t;

    static_assert(std::random_access_iterator<iterator>, "Common iterator isn't random access iterator");


    explicit CircularBufferTraits(const Alloc& allocator = Alloc());

    explicit CircularBufferTraits(size_t size, const Alloc& allocator = Alloc());

    CircularBufferTraits(size_t size, const T& value, const Alloc& allocator = Alloc());

    template<typename LegacyInputIterator>
    CircularBufferTraits(LegacyInputIterator i, LegacyInputIterator j, const Alloc& allocator = Alloc());

    CircularBufferTraits(const std::initializer_list<value_type>& list, const Alloc& allocator = Alloc());

    CircularBufferTraits(const CircularBufferTraits& other);

    CircularBufferTraits(CircularBufferTraits&& other) noexcept;

    CircularBufferTraits& operator=(const CircularBufferTraits& other) noexcept;

    CircularBufferTraits& operator=(CircularBufferTraits&& other) noexcept;

    CircularBufferTraits& operator=(const std::initializer_list<value_type>& list);

    reference operator[](std::size_t i);

    const_reference operator[](std::size_t i) const;

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

    template<typename... Args>
    iterator emplace(iterator p, Args... args);

    iterator insert(iterator p, const_reference value);

    iterator insert(iterator p, value_type&& rv);

    iterator insert(iterator p, size_type n, const_reference value);

    template<typename LegacyInputIterator>
    iterator insert(iterator p, LegacyInputIterator i, LegacyInputIterator j);

    iterator insert(iterator p, const std::initializer_list<value_type>& il);

    iterator erase(const_iterator q);

    iterator erase(const_iterator q1, const_iterator q2);

    void clear() noexcept;

    template<typename LegacyInputIterator>
    void assign(LegacyInputIterator i, LegacyInputIterator j);

    void assign(const std::initializer_list<value_type>& il);

    void assign(size_type n, const_reference value);

    bool operator==(const CircularBufferTraits& other) const noexcept;

    bool operator!=(const CircularBufferTraits& other) const noexcept;

    void swap(CircularBufferTraits& other) noexcept;

    size_type size() const noexcept;

    size_type capacity() const noexcept;

    size_type max_size() const noexcept;

    bool empty() const noexcept;

    void resize(std::size_t n, const value_type& value) noexcept;

    void reserve(size_type n) noexcept;

protected:
    T* buff_start_;
    T* buff_end_;
    T* actual_start_;
    T* actual_end_;
};




template<typename T, typename Alloc>
CircularBufferTraits<T, Alloc>::CircularBufferTraits(const Alloc& allocator)
        :
        allocator_type(allocator),
        buff_start_(AllocTraits::allocate(*this, 1)),
        buff_end_(buff_start_ + 1),
        actual_start_(buff_start_),
        actual_end_(buff_start_) {}


        // TODO remove this constructor
template<typename T, typename Alloc>
CircularBufferTraits<T, Alloc>::CircularBufferTraits(size_t size, const Alloc& allocator) :
        allocator_type(allocator),
        buff_start_(AllocTraits::allocate(*this, size + 1)),
        buff_end_(buff_start_ + size + 1),
        actual_start_(buff_start_),
        actual_end_(buff_start_ + size) {}

template<typename T, typename Alloc>
CircularBufferTraits<T, Alloc>::CircularBufferTraits(size_t size, const T& value, const Alloc& allocator)
        :
        allocator_type(allocator),
        buff_start_(AllocTraits::allocate(*this, size + 1)),
        buff_end_(buff_start_ + size + 1),
        actual_start_(buff_start_),
        actual_end_(buff_end_) {
    size_t current = 0;
    try {
        for (; current < size; ++current) {
            AllocTraits::construct(*this, actual_start_ + current, value);
        }
    } catch (...) {
        for (size_t i = 0; i < current; ++i) {
            AllocTraits::destroy(*this, actual_start_ + i);
        }
        AllocTraits::deallocate(*this, buff_start_, size + 1);
        throw;
    }
}


// TODO same size but not same capacity !!!
template<typename T, typename Alloc>
CircularBufferTraits<T, Alloc>::CircularBufferTraits(const CircularBufferTraits& other)
        :
        allocator_type(AllocTraits::select_on_container_copy_construction(other)),
        buff_start_(AllocTraits::allocate(*this, other.capacity() + 1)),
        buff_end_(buff_start_ + other.capacity() + 1),
        actual_start_(buff_start_),
        actual_end_(actual_start_ + other.size()) {
    std::size_t i = 0;
    try {
        for (; i < other.size(); ++i) {
            AllocTraits::construct(*this, actual_start_ + i, other[i]);
        }
    } catch (...) {
        for (std::size_t j = 0; j < i; ++j) {
            AllocTraits::destroy(*this, actual_start_ + j);
        }
        AllocTraits::deallocate(*this, buff_start_, capacity() + 1);
        throw;
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

        for (auto it = cbegin(); it != cend(); ++it) {
            AllocTraits::destroy(*this, std::addressof(*it));
        }
        AllocTraits::deallocate(*this, buff_start_, std::distance(buff_start_, buff_end_));

        static_cast<allocator_type&>(*this) = std::move(allocator);
        buff_start_ = new_buff_start;
        buff_end_ = new_buff_start + other.size() + 1;
        actual_start_ = buff_start_;
        actual_end_ = buff_end_ - 1;

        return *this;
    }

    T* new_buff_start = AllocTraits::allocate(*this, other.size() + 1);
    try {
        my_uninitialized_copy(other.begin(), other.end(), new_buff_start, *this);
    } catch (...) {
        AllocTraits::deallocate(*this, new_buff_start, other.size() + 1);
        throw;
    }

    for (auto it = begin(); it != end(); ++it) {
        AllocTraits::destroy(*this, std::addressof(*it));
    }
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
        for (auto it = begin(); it != end(); ++it) {
            AllocTraits::destroy(*this, std::addressof(*it));
        }
        AllocTraits::deallocate(*this, buff_start_, std::distance(buff_start_, buff_end_));
        static_cast<allocator_type&>(*this) = std::move(static_cast<allocator_type&>(other));
        buff_start_ = other.buff_start_;
        buff_end_ = other.buff_end_;
        actual_start_ = other.actual_start_;
        actual_end_ = other.actual_end_;

        other.buff_start_ = other.buff_end_ = other.actual_start_ = other.actual_end_ = nullptr;
        return *this;
    }

    //TODO: else
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
CircularBufferTraits<T, Alloc>::reference CircularBufferTraits<T, Alloc>::operator[](std::size_t i) {
    return *(begin() + i);
}

template<typename T, typename Alloc>
CircularBufferTraits<T, Alloc>::const_reference CircularBufferTraits<T, Alloc>::operator[](std::size_t i) const {
    return *(cbegin() + i);
}


//TODO ITERATOR TOOLS
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
void CircularBufferTraits<T, Alloc>::swap(CircularBufferTraits& other) noexcept {
    std::swap(buff_start_, other.buff_start_);
    std::swap(buff_end_, other.buff_end_);
    std::swap(actual_start_, other.actual_start_);
    std::swap(actual_end_, other.actual_end_);
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
void CircularBufferTraits<T, Alloc>::reserve(CircularBufferTraits<T, Alloc>::size_type n) noexcept {
    if (capacity() >= n) {
        return;
    }
    auto new_buff_start = AllocTraits::allocate(*this, n + 1);
    try {
        my_uninitialized_copy(begin(), end(), new_buff_start, *this);
    } catch (...) {
        AllocTraits::deallocate(*this, new_buff_start, n + 1);
    }
    auto old_size = size();
    for (auto it = begin(); it != end(); ++it) {
        AllocTraits::destroy(*this, std::addressof(*it));
    }
    AllocTraits::deallocate(*this, buff_start_, capacity() + 1);

    buff_start_ = new_buff_start;
    buff_end_ = buff_start_ + n + 1;
    actual_start_ = buff_start_;
    actual_end_ = actual_start_ + old_size;
}

