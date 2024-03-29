//
// Created by stefan on 13/03/24.
//

#include <memory>
#include <list>
#include "gtest/gtest.h"

#include "Queue.h"

using data_structures::Queue;

class QueueTest : public ::testing::Test {
protected:
    std::unique_ptr<Queue<int>> queue;

    void SetUp() override {
        queue = std::make_unique<Queue<int> >();
    }

    void TearDown() override {
        queue.reset();
    }
};

TEST_F(QueueTest, QueueAddReadRemoveElement) {
    const std::list initialData = {1, 2, 5, 7, 9};

    ASSERT_EQ(0, queue->size());
    for (auto value: initialData) {
        queue->push(value);
    }
    ASSERT_EQ(initialData.size(), queue->size());

    std::list<int> readValues;
    std::list<int> poppedValues;
    while (!queue->empty()) {
        readValues.emplace_back(queue->front());
        poppedValues.emplace_back(queue->pop());
    }

    ASSERT_EQ(initialData, readValues);
    ASSERT_EQ(initialData, poppedValues);
}
