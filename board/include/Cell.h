//
// Created by stefan on 29/03/24.
//

#ifndef HEX_CELL_H
#define HEX_CELL_H

#include "List.h"

namespace board {

    class Cell {
    public:
        enum class Type {
            empty,
            red,
            blue,
            redBorderRight,
            redBorderLeft,
            blueBorderRight,
            blueBorderLeft,
        };

        explicit Cell(Type mType = Type::empty) : mType(mType) {}

        constexpr Type getType() const {
            return mType;
        }

        void setType(Type type) {
            Cell::mType = type;
        }

        constexpr const data_structures::List<Cell *> &getNeighbours() const {
            return mNeighbours;
        }

        inline void addNeighbour(Cell *neighbour) {
            mNeighbours.pushBack(neighbour);
        }

    private:
        Type mType;
        data_structures::List<Cell *> mNeighbours;
    };

} // board

#endif //HEX_CELL_H
