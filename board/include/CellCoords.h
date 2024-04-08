//
// Created by stefan on 08/04/24.
//

#ifndef HEX_CELLCOORDS_H
#define HEX_CELLCOORDS_H

#include "Cell.h"

namespace board {

    struct CellCoords {
        enum class Direction {
            up,
            down,
            left,
            right,
        };
        size_t row;
        size_t num;
        Cell *parent;
        Direction direction;
    };

} // board

#endif //HEX_CELLCOORDS_H