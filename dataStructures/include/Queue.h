//
// Created by stefan on 13/03/24.
//

#ifndef CALCULATORAADS_QUEUE_H
#define CALCULATORAADS_QUEUE_H

#include "List.h"

namespace data_structures {

    template<typename T>
    class Queue {
    public:
        void push(const T &value) {
            data.pushBack(value);
        }

        T pop() {
            return data.popFront();
        }

        [[nodiscard]] inline T &front() const {
            return data.front();
        }

        [[nodiscard]] inline size_t size() const { return data.size(); }

        [[nodiscard]] inline bool empty() const { return data.empty(); }

        auto begin() const { return data.begin(); }

        auto end() const { return data.end(); }

    private:
        List<T> data;
    };

} // data_structures

#endif //CALCULATORAADS_QUEUE_H
