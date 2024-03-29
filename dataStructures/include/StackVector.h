//
// Created by stefan on 18/03/24.
//

#ifndef CALCULATORAADS_STACKVECTOR_H
#define CALCULATORAADS_STACKVECTOR_H

#include <vector>

namespace data_structures_alternative {

    template<typename T>
    class Stack {
    public:
        void push(const T &value){
            data.push_back(value);
        }

        T pop() {
            T value = data.back();
            data.pop_back();
            return value;
        }

        const T &top() const {
            if (data.empty())
                throw std::out_of_range("read last from empty vector!!");
            return data.back();
        }

        size_t size() const { return data.size(); }

        bool empty() const { return data.empty(); }

        auto begin() const { return data.rbegin(); }

        auto end() const { return data.rend(); }

    private:
        std::vector<T> data;

    };

} // data_structures_alternative

#endif //CALCULATORAADS_STACKVECTOR_H
