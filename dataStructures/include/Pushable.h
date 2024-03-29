//
// Created by stefan on 25/03/24.
//

#ifndef CALCULATORAADS_PUSHABLE_H
#define CALCULATORAADS_PUSHABLE_H

namespace data_structures {

    template<typename T>
    class Pushable {
    public:
        virtual void push(const T &value) = 0;
    };

} // data_structures

#endif //CALCULATORAADS_PUSHABLE_H
