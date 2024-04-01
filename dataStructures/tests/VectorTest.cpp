//
// Created by stefan on 01/04/24.
//

#include <list>
#include "gtest/gtest.h"

#include "Vector.h"

using data_structures::Vector;

class VectorTest : public ::testing::Test {
protected:
    void fillVectorWithSequentNumbers(Vector<int> &vector, int first) {
        auto number = first;
        for (auto i = 0u; i < vector.size(); ++i, ++number) {
            vector[i] = number;
        }
    }
};

TEST_F(VectorTest, vecotrWithSpecifiedSize) {
    const size_t size = 3;
    Vector<int> vector(size);
    const int firstData = 5;

    ASSERT_EQ(size, vector.size());
    fillVectorWithSequentNumbers(vector, firstData);

    for (size_t i = 0; i < size; ++i) {
        ASSERT_EQ(firstData + i, vector[i]);
    }
}

TEST_F(VectorTest, vecotrWithSpecifiedSizeAtMethodTest) {
    const size_t size = 3;
    Vector<int> vector(size);
    const int firstData = -1;
    fillVectorWithSequentNumbers(vector, firstData);

    for (size_t i = 0; i < size; ++i) {
        ASSERT_EQ(firstData + i, vector[i]);
    }
}

TEST_F(VectorTest, lastMethod) {
    const size_t size = 7;
    Vector<int> vector(size);
    const int firstData = 11;
    fillVectorWithSequentNumbers(vector, firstData);

    ASSERT_EQ(firstData + size - 1, vector.last());
}

TEST_F(VectorTest, throwsOutOfRangeExceptionWhenTryToAccessTooBigIndex) {
    const size_t size = 5;
    Vector<int> vector(size);
    ASSERT_THROW(vector[size], std::out_of_range);
}

TEST_F(VectorTest, rangeBasedLoopOverEmptyVector) {
    Vector<int> vector(0);
    size_t loopCounter = 0;
    for (const auto &el: vector) {
        ++loopCounter;
    }
    ASSERT_EQ(0, loopCounter);
}

TEST_F(VectorTest, rangeBasedLoopOverNotEmptyVector) {
    size_t size = 5;
    Vector<int> vector(size);
    size_t loopCounter = 0;
    int firstData = 10;
    fillVectorWithSequentNumbers(vector, firstData);

    auto expecedNumber = firstData;
    for (const auto &el: vector) {
        ASSERT_EQ(expecedNumber, el);
        ++expecedNumber;
        ++loopCounter;
    }
    ASSERT_EQ(size, loopCounter);
}