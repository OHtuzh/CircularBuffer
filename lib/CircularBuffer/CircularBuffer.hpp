#pragma once

#include "Iterator.hpp"

#include <memory>


template<typename T, typename Alloc = std::allocator<T>>
class CircularBuffer : private std::allocator_traits<Alloc>::template rebind_alloc<T> {
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

    using differece_type = iterator::difference_type;
    using size_type = std::size_t;

    static_assert(std::random_access_iterator<iterator>, "Common iterator isn't random access iterator");


    explicit CircularBuffer(const Alloc& allocator = Alloc());

    explicit CircularBuffer(size_t size, const Alloc& allocator = Alloc());

    CircularBuffer(size_t size, const T& value, const Alloc& allocator = Alloc());


    CircularBuffer(const CircularBuffer& other);

    CircularBuffer(CircularBuffer&& other);

    CircularBuffer& operator=(const CircularBuffer& other);

    CircularBuffer& operator=(CircularBuffer&& other);

    ~CircularBuffer();

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

    const_reverse_iterator crbegin() const noexcept;

    const_reverse_iterator crend() const noexcept;

    bool operator==(const CircularBuffer& other) const noexcept;

    bool operator!=(const CircularBuffer& other) const noexcept;

    void swap(const CircularBuffer& other) noexcept;

    size_type size() const noexcept;

    size_type capacity() const noexcept;

    size_type max_size() const noexcept;

    bool empty() const noexcept;

protected:

private:


    T* buff_start_;
    T* buff_end_;
    T* actual_start_;
    T* actual_end_;
};




template<typename T, typename Alloc>
CircularBuffer<T, Alloc>::CircularBuffer(const Alloc& allocator)
        :
        allocator_type(allocator),
        buff_start_(AllocTraits::allocate(*this, 1)),
        buff_end_(buff_start_),
        actual_start_(buff_start_),
        actual_end_(buff_start_) {}


template<typename T, typename Alloc>
CircularBuffer<T, Alloc>::CircularBuffer(size_t size, const Alloc& allocator) :
        allocator_type(allocator),
        buff_start_(AllocTraits::allocate(*this, size + 1)),
        buff_end_(buff_start_ + size + 1),
        actual_start_(nullptr),
        actual_end_(nullptr) {}

template<typename T, typename Alloc>
CircularBuffer<T, Alloc>::CircularBuffer(size_t size, const T& value, const Alloc& allocator)
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

template<typename T, typename Alloc>
CircularBuffer<T, Alloc>::CircularBuffer(const CircularBuffer& other)
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
CircularBuffer<T, Alloc>::CircularBuffer(CircularBuffer&& other)
        :
        allocator_type(std::move(static_cast<allocator_type>(other))),
        buff_start_(other.buff_start_),
        buff_end_(other.buff_end_),
        actual_start_(other.actual_start_),
        actual_end_(other.actual_end_) {
    other.buff_start_ = other.buff_end_ = other.actual_start_ = other.actual_end_ = nullptr;
}

template<typename T, typename Alloc>
CircularBuffer<T, Alloc>& CircularBuffer<T, Alloc>::operator=(const CircularBuffer& other) {
    if (this == &other) {
        return *this;
    }
    T* new_buff_start, new_buff_end, new_actual_start, new_actual_end;

    new_actual_start = new_buff_start = AllocTraits::allocate(*this, other.capacity());
    new_buff_end = new_buff_start + other.capacity() + 1;
    new_actual_end = new_actual_start + other.size();
    std::size_t i = 0;
    try {
        for (; i < other.size(); ++i) {
            AllocTraits::construct(*this, new_actual_start + i, other[i]);
        }
    } catch (...) {
        for (std::size_t j = 0; j < i; ++j) {
            AllocTraits::destroy(*this, new_actual_start + j);
        }
        AllocTraits::deallocate(*this, new_buff_start, other.capacity() + 1);
    }

    for (i = 0; i < size(); ++i) {
        AllocTraits::destroy(*this, buff_start_ + i);
    }
    AllocTraits::deallocate(*this, buff_start_, capacity() + 1); // TODO create propagation

    buff_start_ = new_buff_start;
    buff_end_ = new_buff_end;
    actual_start_ = new_actual_start;
    actual_end_ = new_actual_end;
}

template<typename T, typename Alloc>
CircularBuffer<T, Alloc>::~CircularBuffer() {
    for (std::size_t i = 0; i < size(); ++i) {
        AllocTraits::destroy(*this, buff_start_ + i);
    }

    AllocTraits::deallocate(*this, buff_start_, capacity() + 1);
}

template<typename T, typename Alloc>
CircularBuffer<T, Alloc>::size_type CircularBuffer<T, Alloc>::size() const noexcept {
    return std::distance(cbegin(), cend());
}

template<typename T, typename Alloc>
CircularBuffer<T, Alloc>::size_type CircularBuffer<T, Alloc>::capacity() const noexcept {
    return std::distance(buff_start_, buff_end_) - 1;
}

template<typename T, typename Alloc>
CircularBuffer<T, Alloc>::reference CircularBuffer<T, Alloc>::operator[](std::size_t i) {
    return *(begin() + i);
}

template<typename T, typename Alloc>
CircularBuffer<T, Alloc>::const_reference CircularBuffer<T, Alloc>::operator[](std::size_t i) const {
    return *(cbegin() + i);
}


//TODO ITERATOR TOOLS
template<typename T, typename Alloc>
CircularBuffer<T, Alloc>::iterator CircularBuffer<T, Alloc>::begin() noexcept {
    return CircularBuffer::iterator(actual_start_, buff_start_, buff_end_, actual_start_, actual_end_);
}

template<typename T, typename Alloc>
CircularBuffer<T, Alloc>::iterator CircularBuffer<T, Alloc>::end() noexcept {
    return CircularBuffer::iterator(actual_end_, buff_start_, buff_end_, actual_start_, actual_end_);
}


template<typename T, typename Alloc>
CircularBuffer<T, Alloc>::const_iterator CircularBuffer<T, Alloc>::cbegin() const noexcept {
    return CircularBuffer::const_iterator(actual_start_, buff_start_, buff_end_, actual_start_, actual_end_);
}

template<typename T, typename Alloc>
CircularBuffer<T, Alloc>::const_iterator CircularBuffer<T, Alloc>::cend() const noexcept {
    return CircularBuffer::const_iterator(actual_start_, buff_start_, buff_end_, actual_start_, actual_end_);
}

template<typename T, typename Alloc>
CircularBuffer<T, Alloc>::reverse_iterator CircularBuffer<T, Alloc>::rbegin() noexcept {
    return reverse_iterator(actual_end_, buff_start_, buff_end_, actual_start_, actual_end_);
}

template<typename T, typename Alloc>
CircularBuffer<T, Alloc>::reverse_iterator CircularBuffer<T, Alloc>::rend() noexcept {
    return reverse_iterator(actual_start_, buff_start_, buff_end_, actual_start_, actual_end_);
}

template<typename T, typename Alloc>
CircularBuffer<T, Alloc>::const_reverse_iterator CircularBuffer<T, Alloc>::crbegin() const noexcept {
    return const_reverse_iterator(actual_end_, buff_start_, buff_end_, actual_start_, actual_end_);
}


template<typename T, typename Alloc>
CircularBuffer<T, Alloc>::const_reverse_iterator CircularBuffer<T, Alloc>::crend() const noexcept {
    return const_reverse_iterator(actual_start_, buff_start_, buff_end_, actual_start_, actual_end_);
}

template<typename T, typename Alloc>
bool CircularBuffer<T, Alloc>::operator==(const CircularBuffer& other) const noexcept {
    if (this == &other) {
        return true;
    }
    return std::equal(cbegin(), cend(), other.cbegin(), other.cend());
}

template<typename T, typename Alloc>
void CircularBuffer<T, Alloc>::swap(const CircularBuffer& other) noexcept {

}

template<typename T, typename Alloc>
bool CircularBuffer<T, Alloc>::operator!=(const CircularBuffer& other) const noexcept {
    return !this->operator==(other);
}

