#include "lib/CircularBufferExt.hpp"

#include <gtest/gtest.h>

#include <string>


TEST(PUSH_TEST_EXT, ALTERNATING_PUSH) {
    CircularBufferExt<int> cb(6);
    const CircularBufferExt<int> result({5, 3, 1, 0, 2, 4});
    for (int i = 0; i < 6; ++i) {
        if (i % 2 == 0) {
            cb.push_back(i);
        } else {
            cb.push_front(i);
        }
    }
    ASSERT_TRUE(cb == result);
}

TEST(PUSH_TEST_EXT, PUSHING_COMPLICATED_OBJECTS_WITH_ERASE) {
    CircularBufferExt<std::string> cb = {"aaa", "bbb", "ccc"};
    const CircularBufferExt<std::string> result = {"zzz", "aaa", "bbb", "ccc", "ddd", "eee"};
    cb.reserve(6);
    cb.push_front("zzz");
    cb.push_back("ddd");
    cb.push_back("eee");

    ASSERT_TRUE(cb == result);
}

TEST(PUSH_TEST_EXT, PUSH_WITH_AUTO_EXT) {
    CircularBufferExt<int> cb;
    for (int i = 0; i < 9; ++i) {
        cb.push_back(i);
    }

    ASSERT_TRUE(cb.size() == 9 && cb.capacity() == 16);
}

TEST(POP_TEST_EXT, SIMPLE_POP_TEST_EXT) {
    CircularBufferExt<int> cb(5);
    const CircularBufferExt<int> result = {4, 1, 2};
    cb.push_back(1);
    cb.push_back(2);
    cb.push_back(3);
    cb.push_front(4);
    cb.push_front(5);

    cb.pop_back();
    cb.pop_front();

    ASSERT_TRUE(cb == result);
}

TEST(POP_TEST_EXT, POP_FROM_EMPTY) {
    CircularBufferExt<int> cb(1);
    cb.push_back(1);
    bool is_ub = false;
    cb.pop_back();

    try {
        cb.pop_back();
        is_ub = true;
    } catch (...) {}

    try {
        cb.pop_front();
        is_ub = true;
    } catch (...) {}

    ASSERT_FALSE(is_ub);
}

TEST(ERASE_TEST_EXT, ERASE_ONE_ELEMENT) {
    CircularBufferExt<int> cb = {1, 2, 3, 4, 5};

    cb.erase(cb.cbegin() + 2);

    ASSERT_TRUE(cb == CircularBufferExt<int>({1, 2, 4, 5}));
}

TEST(ERASE_TEST_EXT, ERASE_SEQUENCE) {
    CircularBufferExt<int> cb(5);
    for (int i = 0; i < 5; ++i) {
        if (i % 2 == 0) {
            cb.push_back(i);
        } else {
            cb.push_front(i);
        }
    }
    cb.erase(cb.cbegin() + 1, cb.cend() - 1);

    ASSERT_TRUE(cb == CircularBufferExt<int>({3, 4}));
}

TEST(ASSIGN_TEST_EXT, N_VALUES) {
    CircularBufferExt<int> cb;

    cb.assign(5, 10);

    ASSERT_TRUE(cb == CircularBufferExt<int>({10, 10, 10, 10, 10}));
}

TEST(ASSIGN_TEST_EXT, ASSIGN_ITERATOR) {
    CircularBufferExt<int> cb;
    std::vector<int> v = {1, 2, 3, 4, 5};

    cb.assign(v.begin() + 1, v.end() - 1);

    ASSERT_TRUE(cb == CircularBufferExt<int>({2, 3, 4}));
}

TEST(ASSIGN_TEST_EXT, INITIALIZER_LIST) {
    CircularBufferExt<int> cb = {5, 4, 3, 2, 1};

    cb.assign({1, 2, 3, 4, 5});

    ASSERT_TRUE(cb == CircularBufferExt<int>({1, 2, 3, 4, 5}));
}

TEST(RESERVE_TEST_EXT, SIMPLE_TEST) {
    CircularBufferExt<int> cb = {1, 2, 3, 4, 5};
    CircularBufferExt<int> copy = cb;

    cb.reserve(10);
    ASSERT_TRUE(cb.capacity() == 10 && cb == copy);
}

TEST(RESIZE_TEST_EXT, EXPAND) {
    CircularBufferExt<int> cb = {1, 2, 3, 4, 5};
    cb.resize(7, 6);

    ASSERT_TRUE(cb == CircularBufferExt<int>({1, 2, 3, 4, 5, 6, 6}));
}

TEST(RESIZE_TEST_EXT, SHRINK) {
    CircularBufferExt<int> cb = {1, 2, 3, 4, 5};
    cb.resize(3);

    ASSERT_TRUE(cb == CircularBufferExt<int>({1, 2, 3}));
}

TEST(INSERT_TEST_EXT, INSERT_VALUE) {
    CircularBufferExt<int> cb(5); // {3, 1, 0, 2, 4}
    for (int i = 0; i < 5; ++i) {
        if (i % 2 == 0) {
            cb.push_back(i);
        } else {
            cb.push_front(i);
        }
    }
    cb.insert(cb.begin() + 2, 10);

    ASSERT_TRUE(cb == CircularBufferExt<int>({3, 1, 10, 0, 2, 4}));
}

TEST(INSERT_TEST_EXT, INSERT_N_VALUES) {
    CircularBufferExt<int> cb(5); // {3, 1, 0, 2, 4}
    for (int i = 0; i < 5; ++i) {
        if (i % 2 == 0) {
            cb.push_back(i);
        } else {
            cb.push_front(i);
        }
    }
    cb.insert(cb.begin() + 2, 2, 8);

    ASSERT_TRUE(cb == CircularBufferExt<int>({3, 1, 8, 8, 0, 2, 4}));
}

TEST(INSERT_TEST_EXT, INSERT_ITERATORS) {
    CircularBufferExt<int> cb(5); // {3, 1, 0, 2, 4}
    std::vector<int> v = {1, 2, 3, 4, 5};
    for (int i = 0; i < 5; ++i) {
        if (i % 2 == 0) {
            cb.push_back(i);
        } else {
            cb.push_front(i);
        }
    }
    cb.insert(cb.begin() + 2, v.begin() + 1, v.end() - 1);

    ASSERT_TRUE(cb == CircularBufferExt<int>({3, 1, 2, 3, 4, 0, 2, 4}));
}

TEST(INSERT_TEST_EXT, INSERT_INITIALIZER_LIST) {
    CircularBufferExt<int> cb(5); // {3, 1, 0, 2, 4}
    for (int i = 0; i < 5; ++i) {
        if (i % 2 == 0) {
            cb.push_back(i);
        } else {
            cb.push_front(i);
        }
    }
    cb.insert(cb.begin() + 2, {2, 3, 4});

    ASSERT_TRUE(cb == CircularBufferExt<int>({3, 1, 2, 3, 4, 0, 2, 4}));
}

TEST(FRONT_EXT, SIMPLE_TEST) {
    CircularBufferExt<int> cb; // {3, 1, 0, 2, 4}
    for (int i = 0; i < 5; ++i) {
        if (i % 2 == 0) {
            cb.push_back(i);
        } else {
            cb.push_front(i);
        }
    }

    ASSERT_TRUE(cb.front() == 3);
}

TEST(BACK_EXT, SIMPLE_TEST) {
    CircularBufferExt<int> cb; // {3, 1, 0, 2, 4}
    for (int i = 0; i < 5; ++i) {
        if (i % 2 == 0) {
            cb.push_back(i);
        } else {
            cb.push_front(i);
        }
    }

    ASSERT_TRUE(cb.back() == 4);
}
