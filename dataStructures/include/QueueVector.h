//
// Created by stefan on 18/03/24.
//

#ifndef CALCULATORAADS_QUEUEVECTOR_H
#define CALCULATORAADS_QUEUEVECTOR_H

#include <vector>

namespace data_structures_alternative {

    template<typename T>
    class Queue {
    public:
        void push(const T &value){
            data.push_back(value);
        }

        T pop() {
            T value = data.front();
            data.erase(data.begin());
            return value;
        }

        const T &front() const {
            return data.front();
        }

        size_t size() const { return data.size(); }

        bool empty() const { return data.empty(); }

        auto begin() const { return data.begin(); }

        auto end() const { return data.end(); }

    private:
        std::vector<T> data;
    };

} // data_structures_alternative

#endif //CALCULATORAADS_QUEUEVECTOR_H
