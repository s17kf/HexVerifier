//
// Created by stefan on 10/04/24.
//

#include "MinMax.h"
#include "Vector.h"

namespace board::algorithms {
    typedef data_structures::Vector<data_structures::Vector<size_t>> DistancesType;

    char MinMax::evaluate(
            size_t stepsLeft, PlayerType playerType, Color playerColor, Color opponentColor,
            const data_structures::List<CellCoords> &cellsToPlayByPlayer,
            const data_structures::List<CellCoords> &cellsToPlayByOpponent) {
        if (stepsLeft == 0) {
            if (playerColor == Color::blue) {
                return mBoard.isGameWonByRed(mBfsForRed) ? WIN_VALUE : LOSE_VALUE;
            }
            return mBoard.isGameWonByBlue(mBfsForBlue) ? WIN_VALUE : LOSE_VALUE;
        }
        char bestValue = playerType == PlayerType::min ? WIN_VALUE : LOSE_VALUE;
        for (const auto &coords: cellsToPlayByPlayer) {
            if (mBoard.getColor(coords.row, coords.num) != Color::empty) {
                continue;
            }
            const DistancesType &borderDistances1 =
                    playerColor == Color::red ? mDistanceKeeper.getDistancesToLeftBorder()
                                              : mDistanceKeeper.getDistancesToTopBorder();
            const DistancesType &borderDistances2 =
                    playerColor == Color::red ? mDistanceKeeper.getDistancesToRightBorder()
                                              : mDistanceKeeper.getDistancesToBottomBorder();

            if (borderDistances1[coords.row][coords.num] == 1 &&
                borderDistances2[coords.row][coords.num] == 1) {
                if (playerType == PlayerType::min) {
                    return LOSE_VALUE;
                }
                else {
                    if (stepsLeft != 1)
                        continue;
                    return WIN_VALUE;
                }
            }
            mBoard.setColor(coords.row, coords.num, playerColor);
            PlayerType opponentType = playerType == PlayerType::min ? PlayerType::max : PlayerType::min;
            char newValue = evaluate(stepsLeft - 1, opponentType, opponentColor, playerColor,
                                     cellsToPlayByOpponent, cellsToPlayByPlayer);
            if (playerType == PlayerType::min) {
                if (newValue < bestValue) {
                    mBoard.setColor(coords.row, coords.num, Color::empty);
                    return newValue;
                }
            } else { //  playerType == max
                if (newValue > bestValue) {
                    mBoard.setColor(coords.row, coords.num, Color::empty);
                    return newValue;
                }
            }
            mBoard.setColor(coords.row, coords.num, Color::empty);
        }

        return bestValue;
    }
} // board::algorithms