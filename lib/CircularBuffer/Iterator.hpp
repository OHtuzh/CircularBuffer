#include <iterator>

template<typename U>
struct CommonIterator {
public:
    using difference_type = std::ptrdiff_t;
    using value_type = U;
    using pointer = value_type*;
    using reference = value_type&;
    using iterator_category = std::random_access_iterator_tag;

    CommonIterator();
    CommonIterator(pointer current, pointer buff_start, pointer buff_end);

    reference operator*() const noexcept;
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

    difference_type operator-(const CommonIterator& iterator) const noexcept;

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

//    CommonIterator(T* current, T* buff_start, T* buff_end)
//            :
//            current_(current),
//            buff_start_(buff_start),
//            buff_end_(buff_end) {}
};

template<typename T>
CommonIterator<T>
operator+(int n, const CommonIterator<T>& iter);

static_assert(std::random_access_iterator<CommonIterator<int>>,
              "Common iterator isn't random access iterator");

