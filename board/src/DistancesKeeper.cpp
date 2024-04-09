//
// Created by stefan on 10/04/24.
//

#include "DistancesKeeper.h"
#include "Board.h"
#include "Bfs.h"

using board::algorithms::Bfs;

namespace board {
    DistancesKeeper::DistancesKeeper(const Board &board) : distancesToLeftBorder(board.size()),
                                                           distancesToRightBorder(board.size()),
                                                           distancesToTopBorder(board.size()),
                                                           distancesToBottomBorder(board.size()) {
        Bfs(board).fillDistancesForEmptyCells(distancesToLeftBorder, distancesToRightBorder, distancesToTopBorder,
                                              distancesToBottomBorder);
    }

} // board