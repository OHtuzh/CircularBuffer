#pragma once

#include <iterator>

#define NOT_EMPTY_BUFFER if (buff_start_ + 1 == buff_end_) {return *this;}

template<typename T>
struct CommonIterator {
public:
    using difference_type = std::ptrdiff_t;
    using value_type = T;
    using pointer = value_type*;
    using reference = value_type&;
    using iterator_category = std::random_access_iterator_tag;

    CommonIterator() = default;

    CommonIterator(pointer current, pointer buff_start, pointer buff_end, pointer actual_start, pointer actual_end);

    ~CommonIterator() noexcept = default;

    reference operator*() const noexcept {
        return *current_;
    }

    reference operator->() const noexcept;

    reference operator[](std::size_t n) const noexcept;

    CommonIterator& operator++() noexcept; // infix
    CommonIterator operator++(int) const noexcept; // postfix

    CommonIterator& operator--() noexcept; // infix
    CommonIterator operator--(int) const noexcept; // postfix

    CommonIterator operator+(int n) const noexcept;

    CommonIterator& operator+=(int n) noexcept;

    CommonIterator operator-(int n) const noexcept;

    CommonIterator& operator-=(int n) noexcept;

    difference_type operator-(const CommonIterator<T>& other) const;

    bool operator==(const CommonIterator& other) const noexcept;

    bool operator!=(const CommonIterator& other) const noexcept;

    bool operator>(const CommonIterator& other) const noexcept;

    bool operator>=(const CommonIterator& other) const noexcept;

    bool operator<(const CommonIterator& other) const noexcept;

    bool operator<=(const CommonIterator& other) const noexcept;


private:
    pointer current_;
    pointer buff_start_;
    pointer buff_end_;
    pointer actual_start_;
    pointer actual_end_;
};




template<typename T>
CommonIterator<T>::CommonIterator(CommonIterator::pointer current,
                                  CommonIterator::pointer buff_start,
                                  CommonIterator::pointer buff_end,
                                  CommonIterator::pointer actual_start,
                                  CommonIterator::pointer actual_end)
        :
        current_(current),
        buff_start_(buff_start),
        buff_end_(buff_end),
        actual_start_(actual_start),
        actual_end_(actual_end) {}


template<typename T>
CommonIterator<T>::reference CommonIterator<T>::operator->() const noexcept {
    return current_;
}

template<typename T>
CommonIterator<T>::reference CommonIterator<T>::operator[](std::size_t n) const noexcept {
    return *operator+(n);
}


template<typename T>
CommonIterator<T>& CommonIterator<T>::operator++() noexcept { // infix
    NOT_EMPTY_BUFFER;
    current_ = (current_ + 1 != buff_end_ ? current_ + 1 : buff_start_);
    return *this;
}

template<typename T>
CommonIterator<T> CommonIterator<T>::operator++(int) const noexcept {
    NOT_EMPTY_BUFFER;
    const auto old_ptr = current_;
    current_ = (current_ + 1 != buff_end_ ? current_ + 1 : buff_start_);
    return CommonIterator(old_ptr, buff_start_, buff_end_, actual_start_, actual_end_);
}

template<typename T>
CommonIterator<T>& CommonIterator<T>::operator--() noexcept {
    NOT_EMPTY_BUFFER;
    current_ = (current_ != buff_start_ ? current_ - 1 : buff_end_ - 1);
    return *this;
}


template<typename T>
CommonIterator<T> CommonIterator<T>::operator--(int) const noexcept {
    NOT_EMPTY_BUFFER;
    const auto old_ptr = current_;
    current_ = (current_ != buff_start_ ? current_ - 1 : buff_end_ - 1);
    return CommonIterator(old_ptr, buff_start_, buff_end_, actual_start_, actual_end_);
}

template<typename T>
CommonIterator<T> CommonIterator<T>::operator+(int n) const noexcept {
    NOT_EMPTY_BUFFER;
    if (n == 0) {
        return *this;
    }
    if (n < 0) {
        return this->operator-(-n);
    }
    const auto distance_to_buff_end = std::distance(current_, buff_end_);
    if (n < distance_to_buff_end) {
        return CommonIterator(current_ + n, buff_start_, buff_end_, actual_start_, actual_end_);
    }
    return CommonIterator(buff_start_ + (n - distance_to_buff_end) % std::distance(buff_start_, buff_end_), buff_start_,
                          buff_end_, actual_start_, actual_end_);
}


template<typename T>
CommonIterator<T>& CommonIterator<T>::operator+=(int n) noexcept {
    NOT_EMPTY_BUFFER;
    if (n == 0) {
        return *this;
    }
    if (n < 0) {
        return this->operator-=(-n);
    }
    const auto distance_to_buff_end = std::distance(current_, buff_end_);
    if (n < distance_to_buff_end) {
        current_ += n;
        return *this;
    }
    current_ = buff_start_ + (n - distance_to_buff_end) % std::distance(buff_start_, buff_end_);
    return *this;
}

template<typename T>
CommonIterator<T> CommonIterator<T>::operator-(int n) const noexcept {
    NOT_EMPTY_BUFFER;
    if (n <= 0) {
        return this->operator+(-n);
    }
    const auto distance_to_start = std::distance(buff_start_, current_);
    if (n < distance_to_start) {
        return CommonIterator(current_ - n, buff_start_, buff_end_, actual_start_, actual_end_);
    }
    return CommonIterator(buff_end_ - (n - distance_to_start) % std::distance(buff_start_, buff_end_) - 1, buff_start_,
                          buff_end_, actual_start_, actual_end_);
}

template<typename T>
CommonIterator<T>& CommonIterator<T>::operator-=(int n) noexcept {
    NOT_EMPTY_BUFFER;
    if (n <= 0) {
        return this->operator+=(-n);
    }
    const auto distance_to_start = std::distance(buff_start_, current_);
    if (n <= distance_to_start) {
        return CommonIterator(current_ - n, buff_start_, buff_end_, actual_start_, actual_end_);
    }
    return CommonIterator(buff_end_ - (n - distance_to_start) % std::distance(buff_start_, buff_end_), buff_start_,
                          buff_end_, actual_start_, actual_end_);
}


template<typename T>
typename CommonIterator<T>::difference_type
CommonIterator<T>::operator-(const CommonIterator<T>& other) const { //TODO REWRITE METHOD
    // *this - other
    if (buff_start_ != other.buff_start_ || buff_end_ != other.buff_end_) {
        throw -1; //TODO DIFFERENT ITERATORS EXCEPTION
    }
    if (current_ >= actual_start_) {
        if (other.current_ >= actual_start_) {
            return std::distance(other.current_, current_);
        }
        return -(std::distance(current_, buff_end_) + std::distance(buff_start_, other.current_));
    }
    // Else
    if (other.current_ <= actual_end_) {
        return std::distance(other.current_, current_);
    }
    return std::distance(other.current_, buff_end_) + std::distance(buff_start_, current_);

}

template<typename T>
bool CommonIterator<T>::operator==(const CommonIterator& other) const noexcept {
    return current_ == other.current_;
}

template<typename T>
bool CommonIterator<T>::operator!=(const CommonIterator& other) const noexcept {
    return !(this->operator==(other));
}

template<typename T>
bool CommonIterator<T>::operator>(const CommonIterator& other) const noexcept {
    return (this->operator!=(other) && this->operator-(other) > 0);
}

template<typename T>
bool CommonIterator<T>::operator<(const CommonIterator& other) const noexcept {
    return (this->operator!=(other) && this->operator-(other) < 0);
}

template<typename T>
bool CommonIterator<T>::operator>=(const CommonIterator& other) const noexcept {
    return !(this->operator<(other));
}

template<typename T>
bool CommonIterator<T>::operator<=(const CommonIterator& other) const noexcept {
    return !(this->operator>(other));
}

template<typename T>
CommonIterator<T> operator+(int n, const CommonIterator<T>& iter) {
    return iter.operator+(n);
}



