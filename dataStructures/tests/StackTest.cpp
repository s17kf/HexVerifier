//
// Created by stefan on 13/03/24.
//

#include <memory>
#include <list>
#include "gtest/gtest.h"

#include "Stack.h"

using data_structures::Stack;

class StackTest : public ::testing::Test {
protected:
    std::unique_ptr<Stack<int>> stack;

    void SetUp() override {
        stack = std::make_unique<Stack<int> >();
    }

    void TearDown() override {
        stack.reset();
    }
};

TEST_F(StackTest, StackAddReadRemoveElement) {
    const std::list initialData = {1, 2, 5, 7, 9};
    const std::list reversedData(initialData.rbegin(), initialData.rend());

    ASSERT_EQ(0, stack->size());
    for (auto value: initialData) {
        stack->push(value);
    }
    ASSERT_EQ(initialData.size(), stack->size());

    std::list<int> readValues;
    std::list<int> poppedValues;
    while (!stack->empty()) {
        readValues.emplace_back(stack->top());
        poppedValues.emplace_back(stack->pop());
    }

    ASSERT_EQ(reversedData, readValues);
    ASSERT_EQ(reversedData, poppedValues);
}

TEST_F(StackTest, ReadElementFromEmptyStack) {
    EXPECT_THROW(stack->top(), std::out_of_range);
}
