#include "lib/CircularBuffer.hpp"

#include <gtest/gtest.h>

#include <string>


TEST(PUSH_TEST, ALTERNATING_PUSH) {
    CircularBuffer<int> cb(6);
    const CircularBuffer<int> result({5, 3, 1, 0, 2, 4});
    for (int i = 0; i < 6; ++i) {
        if (i % 2 == 0) {
            cb.push_back(i);
        } else {
            cb.push_front(i);
        }
    }
    ASSERT_TRUE(cb == result);
}

TEST(PUSH_TEST, PUSHING_COMPLICATED_OBJECTS_WITH_ERASE) {
    CircularBuffer<std::string> cb = {"aaa", "bbb", "ccc"};
    const CircularBuffer<std::string> result = {"zzz", "aaa", "bbb", "ccc", "ddd", "eee"};
    cb.reserve(6);
    cb.push_front("zzz");
    cb.push_back("ddd");
    cb.push_back("eee");

    ASSERT_TRUE(cb == result);
}

TEST(POP_TEST, SIMPLE_POP_TEST) {
    CircularBuffer<int> cb(5);
    const CircularBuffer<int> result = {4, 1, 2};
    cb.push_back(1);
    cb.push_back(2);
    cb.push_back(3);
    cb.push_front(4);
    cb.push_front(5);

    cb.pop_back();
    cb.pop_front();

    ASSERT_TRUE(cb == result);
}

TEST(POP_TEST, POP_FROM_EMPTY) {
    CircularBuffer<int> cb(1);
    cb.push_back(1);
    cb.pop_back();

    try {
        cb.pop_back();
        FAIL();
    } catch (...) {}

    try {
        cb.pop_front();
        FAIL();
    } catch (...) {}

    SUCCEED();
}

TEST(ERASE_TEST, ERASE_ONE_ELEMENT) {
    CircularBuffer<int> cb = {1, 2, 3, 4, 5};

    cb.erase(cb.cbegin() + 2);

    ASSERT_TRUE(cb == CircularBuffer<int>({1, 2, 4, 5}));
}

TEST(ERASE_TEST, ERASE_SEQUENCE) {
    CircularBuffer<int> cb(5);
    for (int i = 0; i < 5; ++i) {
        if (i % 2 == 0) {
            cb.push_back(i);
        } else {
            cb.push_front(i);
        }
    }
    cb.erase(cb.cbegin() + 1, cb.cend() - 1);

    ASSERT_TRUE(cb == CircularBuffer<int>({3, 4}));
}

TEST(ASSIGN_TEST, N_VALUES) {
    CircularBuffer<int> cb;

    cb.assign(5, 10);

    ASSERT_TRUE(cb == CircularBuffer<int>({10, 10, 10, 10, 10}));
}

TEST(ASSIGN_TEST, ASSIGN_ITERATOR) {
    CircularBuffer<int> cb;
    std::vector<int> v = {1, 2, 3, 4, 5};

    cb.assign(v.begin() + 1, v.end() - 1);

    ASSERT_TRUE(cb == CircularBuffer<int>({2, 3, 4}));
}

TEST(ASSIGN_TEST, INITIALIZER_LIST) {
    CircularBuffer<int> cb = {5, 4, 3, 2, 1};

    cb.assign({1, 2, 3, 4, 5});

    ASSERT_TRUE(cb == CircularBuffer<int>({1, 2, 3, 4, 5}));
}

TEST(RESERVE_TEST, SIMPLE_TEST) {
    CircularBuffer<int> cb = {1, 2, 3, 4, 5};
    CircularBuffer<int> copy = cb;

    cb.reserve(10);
    ASSERT_EQ(cb.capacity(), 10);
    ASSERT_TRUE(cb == copy);
}

TEST(RESIZE_TEST, EXPAND) {
    CircularBuffer<int> cb = {1, 2, 3, 4, 5};
    cb.resize(7, 6);

    ASSERT_TRUE(cb == CircularBuffer<int>({1, 2, 3, 4, 5, 6, 6}));
}

TEST(RESIZE_TEST, SHRINK) {
    CircularBuffer<int> cb = {1, 2, 3, 4, 5};
    cb.resize(3);

    ASSERT_TRUE(cb == CircularBuffer<int>({1, 2, 3}));
}

TEST(INSERT_TEST, INSERT_VALUE) {
    CircularBuffer<int> cb(5); // {3, 1, 0, 2, 4}
    for (int i = 0; i < 5; ++i) {
        if (i % 2 == 0) {
            cb.push_back(i);
        } else {
            cb.push_front(i);
        }
    }
    cb.insert(cb.begin() + 2, 10);

    ASSERT_TRUE(cb == CircularBuffer<int>({3, 1, 10, 0, 2, 4}));
}

TEST(INSERT_TEST, INSERT_N_VALUES) {
    CircularBuffer<int> cb(5); // {3, 1, 0, 2, 4}
    for (int i = 0; i < 5; ++i) {
        if (i % 2 == 0) {
            cb.push_back(i);
        } else {
            cb.push_front(i);
        }
    }
    cb.insert(cb.begin() + 2, 2, 8);

    ASSERT_TRUE(cb == CircularBuffer<int>({3, 1, 8, 8, 0, 2, 4}));
}

TEST(INSERT_TEST, INSERT_ITERATORS) {
    CircularBuffer<int> cb(5); // {3, 1, 0, 2, 4}
    std::vector<int> v = {1, 2, 3, 4, 5};
    for (int i = 0; i < 5; ++i) {
        if (i % 2 == 0) {
            cb.push_back(i);
        } else {
            cb.push_front(i);
        }
    }
    cb.insert(cb.begin() + 2, v.begin() + 1, v.end() - 1);

    ASSERT_TRUE(cb == CircularBuffer<int>({3, 1, 2, 3, 4, 0, 2, 4}));
}

TEST(INSERT_TEST, INSERT_INITIALIZER_LIST) {
    CircularBuffer<int> cb(5); // {3, 1, 0, 2, 4}
    for (int i = 0; i < 5; ++i) {
        if (i % 2 == 0) {
            cb.push_back(i);
        } else {
            cb.push_front(i);
        }
    }
    cb.insert(cb.begin() + 2, {2, 3, 4});

    ASSERT_TRUE(cb == CircularBuffer<int>({3, 1, 2, 3, 4, 0, 2, 4}));
}

TEST(FRONT, SIMPLE_TEST) {
    CircularBuffer<int> cb(5); // {3, 1, 0, 2, 4}
    for (int i = 0; i < 5; ++i) {
        if (i % 2 == 0) {
            cb.push_back(i);
        } else {
            cb.push_front(i);
        }
    }

    ASSERT_TRUE(cb.front() == 3);
}

TEST(BACK, SIMPLE_TEST) {
    CircularBuffer<int> cb(5); // {3, 1, 0, 2, 4}
    for (int i = 0; i < 5; ++i) {
        if (i % 2 == 0) {
            cb.push_back(i);
        } else {
            cb.push_front(i);
        }
    }

    ASSERT_TRUE(cb.back() == 4);
}
