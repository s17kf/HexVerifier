//
// Created by stefan on 01/04/24.
//

#ifndef HEX_VECTOR_H
#define HEX_VECTOR_H

#include <iterator>

namespace data_structures {

    template<typename T>
    class Vector {
    public:
        class Iterator {
        public:
            using iterator_category = std::forward_iterator_tag;
            using difference_type = std::ptrdiff_t;
            using value_type = T;
            using pointer = value_type *;
            using reference = value_type &;

            reference operator*() const { return *mPtr; }

            pointer operator->() { return mPtr; }

            Iterator &operator++() {
                ++mPtr;
                return *this;
            }

            Iterator operator++(int) {
                Iterator tmp = *this;
                ++mPtr;
                return tmp;
            }

            bool operator==(const Iterator &other) const { return mPtr == other.mPtr; }

            bool operator!=(const Iterator &other) const { return !(*this == other); }

            explicit Iterator(pointer ptr) : mPtr(ptr) {}

        protected:
            pointer mPtr;
        };

        explicit Vector(size_t size) : mSize(size) {
            data = new T[size];
        }

        virtual ~Vector() {
            delete[] data;
        }

        Iterator begin() const {
            return Iterator(&data[0]);
        }

        Iterator end() const {
            return Iterator(&data[size()]);
        }

        T &operator[](size_t index) {
            return at(index);
        }

        const T&operator[](size_t index) const {
            return at(index);
        }

        inline T &at(size_t index) {
            if (index < mSize)
                return data[index];
            throw std::out_of_range("vector operator[] out of range!");
        }

        inline const T &at(size_t index) const {
            if (index < mSize)
                return data[index];
            throw std::out_of_range("vector operator[] out of range!");
        }

        const T& last() const {
            return data[size() - 1];
        }

        inline size_t size() const {
            return mSize;
        }

    private:
        size_t mSize;
        T *data;
    };

} // data_structures

#endif //HEX_VECTOR_H
