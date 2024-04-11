//
// Created by stefan on 12/03/24.
//

#ifndef LIST_H
#define LIST_H

#include <iterator>

namespace data_structures {
    template<typename T>
    class List {
        struct Node {
            explicit Node(T data = T(), Node *prev = nullptr, Node *next = nullptr) :
                    data(data), prev(prev), next(next) {}

            T data;
            Node *prev;
            Node *next;
        };

    public:
        class Iterator {
        public:
            using iterator_category = std::forward_iterator_tag;
            using difference_type = std::ptrdiff_t;
            using value_type = T;
            using pointer = value_type *;
            using reference = value_type &;

            reference operator*() const { return node->data; }

            pointer operator->() { return &node->data; }

            Iterator &operator++() {
                node = node->next;
                return *this;
            }

            Iterator operator++(int) {
                Iterator tmp = *this;
                node = node->next;
                return tmp;
            }

            bool operator==(const Iterator &other) const { return node == other.node; }

            bool operator!=(const Iterator &other) const { return !(*this == other); }

            explicit Iterator(Node *node) : node(node) {}

        protected:
            Node *node;
        };

        class ReverseIterator {
        public:
            using iterator_category = std::forward_iterator_tag;
            using difference_type = std::ptrdiff_t;
            using value_type = T;
            using pointer = value_type *;
            using reference = value_type &;

            reference operator*() const { return node->data; }

            pointer operator->() { return &node->data; }

            ReverseIterator &operator++() {
                node = node->prev;
                return *this;
            }

            ReverseIterator operator++(int) {
                ReverseIterator tmp = *this;
                node = node->prev;
                return tmp;
            }

            bool operator==(const ReverseIterator &other) const { return node == other.node; }

            bool operator!=(const ReverseIterator &other) const { return !(*this == other); }

            explicit ReverseIterator(Node *node) : node(node) {}

        protected:
            Node *node;
        };


        constexpr explicit List() : mSize(0) {
            setUpEmptyListNodes();
        }

        List(const List<T> &other) = delete;

        List<T> &operator=(const List<T> &other) = delete;

        ~List() {
            Node *node = beforeFirst;
            while (node != nullptr) {
                const Node *toDelete = node;
                node = node->next;
                delete toDelete;
            }
        }

        Iterator begin() const { return ++Iterator(beforeFirst); }

        Iterator end() const { return Iterator(afterLast); }

        ReverseIterator rbegin() const { return ++ReverseIterator(afterLast); }

        ReverseIterator rend() const { return ReverseIterator(beforeFirst); }

        [[nodiscard]] constexpr size_t size() const { return mSize; }

        [[nodiscard]] constexpr bool empty() const { return mSize == 0; }

        void pushBack(const T &data) {
            Node *newNode = new Node(data, afterLast->prev, afterLast);
            afterLast->prev->next = newNode;
            afterLast->prev = newNode;
            ++mSize;
        }

        void pushFront(const T &data) {
            Node *newNode = new Node(data, beforeFirst, beforeFirst->next);
            beforeFirst->next->prev = newNode;
            beforeFirst->next = newNode;
            ++mSize;
        }

        [[nodiscard]] constexpr T &front() const {
            if (mSize == 0) {
                throw std::out_of_range("Read (first) element from empty list!");
            }
            return beforeFirst->next->data;
        }

        [[nodiscard]] constexpr T &back() const {
            if (mSize == 0) {
                throw std::out_of_range("Read (last) element from empty list!");
            }
            return afterLast->prev->data;
        }

        T popFront() {
            if (mSize == 0) {
                throw std::out_of_range("Try to remove (first) element from empty list!");
            }
            T data = beforeFirst->next->data;
            Node *firstNode = beforeFirst->next;
            beforeFirst->next = firstNode->next;
            firstNode->next->prev = beforeFirst;
            delete firstNode;
            --mSize;
            return data;
        }

        T popBack() {
            if (mSize == 0) {
                throw std::out_of_range("Try to remove (last) element from empty list!");
            }
            T data = afterLast->prev->data;
            Node *lastNode = afterLast->prev;
            afterLast->prev = lastNode->prev;
            lastNode->prev->next = afterLast;
            delete lastNode;
            --mSize;
            return data;
        }

    private:
        void setUpEmptyListNodes() {
            beforeFirst = new Node();
            afterLast = new Node();
            beforeFirst->next = afterLast;
            afterLast->prev = beforeFirst;
        }

        size_t mSize;
        Node *beforeFirst;
        Node *afterLast;
    };
} // data_structures

#endif //LIST_H
