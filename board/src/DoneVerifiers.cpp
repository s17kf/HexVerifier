//
// Created by stefan on 09/04/24.
//

#include "DoneVerifiers.h"

#include "Board.h"

namespace board {

    bool RedDoneVerifier::operator()(const CellCoords *cellCoords) const {
        return cellCoords->num == mBoard.size() - 1;
    }

    bool BlueDoneVerifier::operator()(const CellCoords *cellCoords) const {
        return cellCoords->row == mBoard.size() - 1;
    }

} // board