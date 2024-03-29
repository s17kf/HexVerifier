//
// Created by stefan on 12/03/24.
//

#include <list>
#include <memory>
#include "gtest/gtest.h"

#include "List.h"

using data_structures::List;

class ListTest : public ::testing::Test {
protected:
    std::unique_ptr<List<int>> list;

    void SetUp() override {
        list = std::make_unique<List<int> >();
    }

    void TearDown() override {
        list.reset();
    }
};

TEST_F(ListTest, emptyListTest) {
    ASSERT_EQ(0, list->size());
    ASSERT_TRUE(list->empty());
}

TEST_F(ListTest, rangeBasedLoopOverEmptyList) {
    size_t loopCounter = 0;
    for (const auto &el: *list) {
        ++loopCounter;
    }
    ASSERT_EQ(0, loopCounter);
}

TEST_F(ListTest, notEmptyListSize) {
    ASSERT_TRUE(list->empty());

    list->pushBack(1);
    ASSERT_FALSE(list->empty());

    list->pushBack(2);
    ASSERT_FALSE(list->empty());

    list->pushBack(5);
    ASSERT_FALSE(list->empty());

    list->pushFront(6);
    ASSERT_FALSE(list->empty());
    ASSERT_EQ(4, list->size());
}

TEST_F(ListTest, listWithElementsAddedByPushBack) {
    const std::list expectedList = {1, 2, 5};
    std::list<int> actualList;

    list->pushBack(1);
    list->pushBack(2);
    list->pushBack(5);

    for (const auto &el: *list) {
        actualList.emplace_back(el);
    }
    ASSERT_EQ(expectedList, actualList);
}

TEST_F(ListTest, listWithElementsAddedByPushFront) {
    const std::list expectedList = {5, 2, 1};
    std::list<int> actualList;

    list->pushFront(1);
    list->pushFront(2);
    list->pushFront(5);

    for (const auto &el: *list) {
        actualList.emplace_back(el);
    }
    ASSERT_EQ(expectedList, actualList);
}

TEST_F(ListTest, listWithElementsAddedByPushFrontAndPushBack) {
    const std::list expectedList = {5, 2, 1, 7};
    std::list<int> actualList;

    list->pushBack(1);
    list->pushFront(2);
    list->pushFront(5);
    list->pushBack(7);

    for (const auto &el: *list) {
        actualList.emplace_back(el);
    }
    ASSERT_EQ(expectedList, actualList);
}

TEST_F(ListTest, listBackwardsIteration) {
    const std::list expectedList = {5, 2, 1};
    std::list<int> actualList;

    list->pushBack(1);
    list->pushBack(2);
    list->pushBack(5);

    for (auto it = list->rbegin(); it != list->rend(); ++it) {
        actualList.emplace_back(*it);
    }
    ASSERT_EQ(expectedList, actualList);
}

TEST_F(ListTest, getFirstElementFromList) {
    list->pushBack(1);
    list->pushBack(2);
    list->pushBack(5);

    ASSERT_EQ(1, list->front());
}

TEST_F(ListTest, getLastElementFromList) {
    list->pushBack(1);
    list->pushBack(2);
    list->pushBack(5);

    ASSERT_EQ(5, list->back());
}

TEST_F(ListTest, getFirstElementFromEmptyListShouldThrow) {
    ASSERT_THROW(list->front(), std::out_of_range);
}

TEST_F(ListTest, getLastElementFromEmptyListShouldThrow) {
    ASSERT_THROW(list->back(), std::out_of_range);
}

TEST_F(ListTest, popFrontElement) {
    list->pushBack(1);
    list->pushBack(2);
    list->pushBack(5);

    ASSERT_EQ(1, list->popFront());
    ASSERT_EQ(2, list->front());

    ASSERT_EQ(2, list->popFront());
    ASSERT_EQ(5, list->front());
    ASSERT_EQ(1, list->size());
}


TEST_F(ListTest, popBackElement) {
    list->pushBack(1);
    list->pushBack(2);
    list->pushBack(5);

    ASSERT_EQ(5, list->popBack());
    ASSERT_EQ(2, list->back());

    ASSERT_EQ(2, list->popBack());
    ASSERT_EQ(1, list->back());
    ASSERT_EQ(1, list->size());
}

TEST_F(ListTest, popFrontFromEmptyListShouldThrow) {
    ASSERT_THROW(list->popFront(), std::out_of_range);
}

TEST_F(ListTest, popBackFromEmptyListShouldThrow) {
    ASSERT_THROW(list->popBack(), std::out_of_range);
}
