//
// Created by stefan on 29/03/24.
//

#ifndef HEX_CELL_H
#define HEX_CELL_H

#include <cstdint>
#include "List.h"

namespace board {

    class Cell {
    public:
        enum class Type {
            empty,
            red,
            blue
        };

        explicit Cell(Type mType = Type::empty) : mType(mType) {}

        constexpr Type getType() const {
            return mType;
        }

        void setType(Type type) {
            Cell::mType = type;
        }

    private:
        Type mType;
    };

} // board

#endif //HEX_CELL_H
