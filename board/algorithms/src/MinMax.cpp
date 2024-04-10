//
// Created by stefan on 10/04/24.
//

#include <climits>
#include "MinMax.h"
#include "Vector.h"

namespace board::algorithms {
    typedef data_structures::Vector<data_structures::Vector<size_t>> DistancesType;

    char MinMax::evaluate(
            size_t stepsLeft, PlayerType playerType, Cell::Type playerColor, Cell::Type opponentColor,
            const data_structures::List<CellCoords> &cellsToPlayByPlayer,
            const data_structures::List<CellCoords> &cellsToPlayByOpponent) {
        if (stepsLeft == 0) {
            if (playerColor == Cell::Type::blue) {
                return mBoard.isGameWonByRed(mBfsForRed) ? WIN_VALUE : LOSE_VALUE;
            }
            return mBoard.isGameWonByBlue(mBfsForBlue) ? WIN_VALUE : LOSE_VALUE;
        }
        char bestValue = playerType == PlayerType::min ? WIN_VALUE : LOSE_VALUE;
        for (const auto &coords: cellsToPlayByPlayer) {
            if (mBoard.getType(coords.row, coords.num) != Cell::Type::empty) {
                continue;
            }
            const DistancesType &borderDistances1 =
                    playerColor == Cell::Type::red ? mDistanceKeeper.getDistancesToLeftBorder()
                                                   : mDistanceKeeper.getDistancesToTopBorder();
            const DistancesType &borderDistances2 =
                    playerColor == Cell::Type::red ? mDistanceKeeper.getDistancesToRightBorder()
                                                   : mDistanceKeeper.getDistancesToBottomBorder();

            if (playerType == PlayerType::min && borderDistances1[coords.row][coords.num] == 1 &&
                borderDistances2[coords.row][coords.num] == 1) {
                return LOSE_VALUE;
            }
            mBoard.setType(coords.row, coords.num, playerColor);
            PlayerType opponentType = playerType == PlayerType::min ? PlayerType::max : PlayerType::min;
            char newValue = evaluate(stepsLeft - 1, opponentType, opponentColor, playerColor,
                                     cellsToPlayByOpponent, cellsToPlayByPlayer);
            if (playerType == PlayerType::min) {
                if (newValue < bestValue) {
//                    bestValue = newValue;
                    mBoard.setType(coords.row, coords.num, Cell::Type::empty);
                    return newValue;
                }
            } else { //  playerType == max
                if (newValue > bestValue) {
//                    bestValue = newValue;
                    if (stepsLeft > 2) {
                        if (playerColor == Cell::Type::red) {
                            if (mBoard.isGameWonByRed(mBfsForRed)) {
                                mBoard.setType(coords.row, coords.num, Cell::Type::empty);
                                continue;
                            }
                        } else {
                            if (mBoard.isGameWonByBlue(mBfsForBlue)) {
                                mBoard.setType(coords.row, coords.num, Cell::Type::empty);
                                continue;
                            }
                        }
                    }
                    mBoard.setType(coords.row, coords.num, Cell::Type::empty);
                    return newValue;
                }
            }
            mBoard.setType(coords.row, coords.num, Cell::Type::empty);
        }

        return bestValue;
    }
} // board::algorithms