//
// Created by stefan on 10/04/24.
//

#ifndef HEX_MINMAX_H
#define HEX_MINMAX_H

#include "Board.h"
#include "DistancesKeeper.h"
#include "List.h"
#include "Bfs.h"
#include "Dfs.h"
#include "CellCoords.h"

namespace board::algorithms {

    class MinMax {
    public:
        static const char WIN_VALUE = 1;
        static const char LOSE_VALUE = -1;

        enum class PlayerType {
            min,
            max,
        };

        MinMax(Board &board, const DistancesKeeper &distanceKeeper, const Bfs &bfsForRed, const Bfs &bfsForBlue)
                : mBoard(board), mDistanceKeeper(distanceKeeper), mBfsForRed(bfsForRed), mBfsForBlue(bfsForBlue) {}

        char evaluate(size_t stepsLeft, PlayerType playerType, Color playerColor, Color opponentColor,
                      const data_structures::List<CellCoords> &cellsToPlayByPlayer,
                      const data_structures::List<CellCoords> &cellsToPlayByOpponent);

    private:
        Board &mBoard;
        const DistancesKeeper &mDistanceKeeper;
        const Bfs &mBfsForRed;
        const Bfs &mBfsForBlue;
    };

} // board::algorithms

#endif //HEX_MINMAX_H
