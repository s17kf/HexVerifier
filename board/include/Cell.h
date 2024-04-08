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
            blue,
            redBorderRight,
            redBorderLeft,
            blueBorderRight,
            blueBorderLeft,
        };

        explicit Cell(Type type = Type::empty) : mType(type), mTemporaryColor(true) {}

        inline bool isTemporary() const {
            return mTemporaryColor;
        }

        inline void markNotTemporary() {
            mTemporaryColor = false;
        }

        inline Type getType() const {
            return mType;
        }

        inline void setType(Type type) {
            Cell::mType = type;
        }

        inline const data_structures::List<Cell *> &getNeighbours() const {
            return mNeighbours;
        }

        inline void addNeighbour(Cell *neighbour) {
            mNeighbours.pushBack(neighbour);
        }

        bool visited = false;
        Cell* parent = nullptr;
        size_t closestBlue = SIZE_MAX;
        size_t closestRed = SIZE_MAX;
        bool isStartLeaf = false;
        bool isEndLeaf = false;
    private:
        Type mType;
        data_structures::List<Cell *> mNeighbours;
        bool mTemporaryColor;
    };

} // board

#endif //HEX_CELL_H
