//
// Created by stefan on 01/04/24.
//

#include <climits>
#include "Board.h"

#include "DoneVerifiers.h"
#include "NeighboursHelpers.h"
#include "Dfs.h"
#include "Bfs.h"
#include "MinMax.h"


using data_structures::Vector;
using data_structures::List;

namespace board {
    using algorithms::Dfs;
    using algorithms::Bfs;
    using algorithms::WinVerificationAlgorithm;
    using algorithms::MinMax;

    Board::Board(size_t size) :
            mSize(size),
            mRedCellsCount(0),
            mBlueCellsCount(0),
            mBoard(size),
            mRedDoneVerifier(*this),
            mBlueDoneVerifier(*this),
            mRedNeighboursHelper(*this),
            mBlueNeighboursHelper(*this),
            mRedStraightNeighboursHelper(*this),
            mBlueStraightNeighboursHelper(*this),
            mBfsForRed(*this, mRedStraightNeighboursHelper, mRedDoneVerifier),
            mBfsForBlue(*this, mBlueStraightNeighboursHelper, mBlueDoneVerifier) {
        for (auto &row: mBoard) {
            row.init(size);
        }
    }


    size_t Board::getColorCount(Color color) const {
        switch (color) {
            case Color::red:
                return mRedCellsCount;
            case Color::blue:
                return mBlueCellsCount;
            case Color::empty:
                return mSize * mSize - mRedCellsCount - mBlueCellsCount;
        }
        throw std::invalid_argument("Get color count - invalid color!");
    }


    bool Board::isBoardCorrect() const {
        auto redPawns = getColorCount(Color::red);
        auto bluePawns = getColorCount(Color::blue);
        if (redPawns < bluePawns || redPawns > bluePawns + 1) {
            return false;
        }
        return true;
    }

    bool Board::isGameWonByRed(const DistancesKeeper &distancesKeeper) const {
        return isGameWonByRed(distancesKeeper.getDistancesToRightBorder());
    }

    bool Board::isGameWonByBlue(const DistancesKeeper &distancesKeeper) const {
        return isGameWonByBlue(distancesKeeper.getDistancesToBottomBorder());
    }

    bool Board::isGameWonByRed(const Board::DistancesType &distancesToRightBorder) const {
        for (size_t row = 0; row < size(); ++row) {
            if (distancesToRightBorder[row][0] == 0)
                return true;
        }
        return false;
    }

    bool Board::isGameWonByBlue(const Board::DistancesType &distancesTpBottomBorder) const {
        for (size_t num = 0; num < size(); ++num) {
            if (distancesTpBottomBorder[0][num] == 0)
                return true;
        }
        return false;
    }

    bool Board::isGameWonByRed(const WinVerificationAlgorithm &algorithm) const {
        List<CellCoords> path;
        return isGameWonByRed(algorithm, path);
    }

    bool Board::isGameWonByBlue(const WinVerificationAlgorithm &algorithm) const {
        List<CellCoords> path;
        return isGameWonByBlue(algorithm, path);
    }

    bool Board::isGameWonByRed(const WinVerificationAlgorithm &algorithm, List <CellCoords> &path) const {
        List<CellCoords *> startCoordsList;
        for (size_t row = 0u; row < size(); ++row) {
            if (getColor(row, 0) == Color::red)
                startCoordsList.pushBack(new CellCoords{row, 0, CellCoords::Direction::right});
        }
        return algorithm(startCoordsList, path);
    }

    bool Board::isGameWonByBlue(const WinVerificationAlgorithm &algorithm, List <CellCoords> &path) const {
        List<CellCoords *> startCoordsList;
        for (size_t num = 0u; num < size(); ++num) {
            if (getColor(0, num) == Color::blue)
                startCoordsList.pushBack(new CellCoords{0, num, CellCoords::Direction::left});
        }
        return algorithm(startCoordsList, path);
    }

    bool Board::isBoardPossible() const {
        if (!isBoardCorrect())
            return false;
        {
            List<CellCoords> redPath;
            const Dfs dfs(*this, mRedNeighboursHelper, mRedDoneVerifier);
            if (isGameWonByRed(dfs, redPath)) {
                if (mBlueCellsCount == mRedCellsCount)
                    return false;
                const Bfs bfs(*this, mRedStraightNeighboursHelper, mRedDoneVerifier);
                return !isGameWonByRed(bfs, redPath);
            }
        }
        {
            List<CellCoords> bluePath;
            const Dfs dfs(*this, mBlueNeighboursHelper, mBlueDoneVerifier);
            if (isGameWonByBlue(dfs, bluePath)) {
                if (mBlueCellsCount < mRedCellsCount)
                    return false;
                const Bfs bfs(*this, mBlueStraightNeighboursHelper, mBlueDoneVerifier);
                return !isGameWonByBlue(bfs, bluePath);
            }
        }
        return true;
    }

    bool Board::canRedWinInNMovesWithNaive(size_t n, const DistancesKeeper &distancesKeeper) {
        if (isGameWonByRed(distancesKeeper.getDistancesToRightBorder()) ||
            isGameWonByBlue(distancesKeeper.getDistancesToBottomBorder())) {
            return false;
        }
        bool movesFirst = mRedCellsCount == mBlueCellsCount;
        return canWinInNMovesWithNaive(n, movesFirst, distancesKeeper.getDistancesToLeftBorder(),
                                       distancesKeeper.getDistancesToRightBorder());
    }

    bool Board::canBlueWinInNMovesWithNaive(size_t n, const DistancesKeeper &distancesKeeper) {
        if (isGameWonByRed(distancesKeeper.getDistancesToRightBorder()) ||
            isGameWonByBlue(distancesKeeper.getDistancesToBottomBorder())) {
            return false;
        }
        bool movesFirst = mRedCellsCount > mBlueCellsCount;
        return canWinInNMovesWithNaive(n, movesFirst, distancesKeeper.getDistancesToTopBorder(),
                                       distancesKeeper.getDistancesToBottomBorder());
    }

    bool Board::canWinInNMovesWithNaive(size_t n, bool movesFirst, const Board::DistancesType &distancesToFirstBorder,
                                        const Board::DistancesType &distancesToSecondBorder) {
        size_t neededEmptyCells = movesFirst ? 2 * n - 1 : 2 * n;
        if (neededEmptyCells > getColorCount(Color::empty) || !isBoardCorrect())
            return false;
        List<CellCoords> playerPossibleMoves;
        fillEmptyCellsForPlayer(playerPossibleMoves, distancesToFirstBorder, distancesToSecondBorder, n);
        if (playerPossibleMoves.size() < n)
            return false;
        if (n == 1) {
            bool winningCellExists = false;
            for (const auto &coords: playerPossibleMoves) {
                if (isOneMoveWinningCell(distancesToFirstBorder, distancesToSecondBorder, coords.row, coords.num)) {
                    winningCellExists = true;
                    break;
                }
            }
            return winningCellExists;
        }
        bool oneMoveWinningCellExists = false;
        bool idleMoveExists = false;
        for (const auto &coords: playerPossibleMoves) {
            if (isTwoMovesWinningCell(distancesToFirstBorder, distancesToSecondBorder, coords.row, coords.num)) {
                return true;
            }
            if (isOneMoveWinningCell(distancesToFirstBorder, distancesToSecondBorder, coords.row, coords.num)) {
                oneMoveWinningCellExists = true;
            } else {
                idleMoveExists = true;
            }
            if (oneMoveWinningCellExists && idleMoveExists)
                return true;
        }
        return false;
    }

    bool Board::canRedWinInNMovesWithPerfect(size_t n, const DistancesKeeper &distancesKeeper) {
        if (isGameWonByRed(distancesKeeper.getDistancesToRightBorder()) ||
            isGameWonByBlue(distancesKeeper.getDistancesToBottomBorder())) {
            return false;
        }
        bool movesFirst = mRedCellsCount == mBlueCellsCount;
        MinMax minMax(*this, distancesKeeper, mBfsForRed, mBfsForBlue);
        return canWinInNMovesWithPerfect(n, movesFirst, Color::red, Color::blue, minMax,
                                         distancesKeeper.getDistancesToLeftBorder(),
                                         distancesKeeper.getDistancesToRightBorder(),
                                         distancesKeeper.getDistancesToTopBorder(),
                                         distancesKeeper.getDistancesToBottomBorder());
    }

    bool Board::canBlueWinInNMovesWithPerfect(size_t n, const DistancesKeeper &distancesKeeper) {
        if (isGameWonByRed(distancesKeeper.getDistancesToRightBorder()) ||
            isGameWonByBlue(distancesKeeper.getDistancesToBottomBorder())) {
            return false;
        }
        bool movesFirst = mRedCellsCount > mBlueCellsCount;
        MinMax minMax(*this, distancesKeeper, mBfsForRed, mBfsForBlue);
        return canWinInNMovesWithPerfect(n, movesFirst, Color::blue, Color::red, minMax,
                                         distancesKeeper.getDistancesToTopBorder(),
                                         distancesKeeper.getDistancesToBottomBorder(),
                                         distancesKeeper.getDistancesToLeftBorder(),
                                         distancesKeeper.getDistancesToRightBorder());
    }

    bool Board::canWinInNMovesWithPerfect(size_t n, bool movesFirst, Color playerColor, Color opponentColor,
                                          algorithms::MinMax &minMax,
                                          const Board::DistancesType &playerDistances1,
                                          const Board::DistancesType &playerDistances2,
                                          const Board::DistancesType &opponentDistances1,
                                          const Board::DistancesType &opponentDistances2) {
        size_t neededSteps = movesFirst ? 2 * n - 1 : 2 * n;
        if (neededSteps > getColorCount(Color::empty) || !isBoardCorrect()) {
            return false;
        }
        List<CellCoords> playerPossibleMoves;
        fillEmptyCellsForPlayer(playerPossibleMoves, playerDistances1, playerDistances2, n);
        if (playerPossibleMoves.size() < n)
            return false;
        bool winningCellExists = false;
        for (const auto &coords: playerPossibleMoves) {
            if (canBeWinningCell(playerDistances1, playerDistances2, n, coords.row, coords.num)) {
                winningCellExists = true;
                break;
            }
        }
        if (!winningCellExists) {
            return false;
        }
        size_t opponentMoves = movesFirst ? n - 1 : n;
        List<CellCoords> opponentPossibleMoves;
        fillEmptyCellsForOpponent(opponentPossibleMoves, playerDistances1, playerDistances2, opponentDistances1,
                                  opponentDistances2, n, opponentMoves);
        if (movesFirst) {
            return MinMax::WIN_VALUE == minMax.evaluate(
                    neededSteps, MinMax::PlayerType::max, playerColor, opponentColor,
                    playerPossibleMoves, opponentPossibleMoves);
        } else {
            return MinMax::WIN_VALUE == minMax.evaluate(
                    neededSteps, algorithms::MinMax::PlayerType::min, opponentColor, playerColor,
                    opponentPossibleMoves, playerPossibleMoves);
        }
    }

    void Board::setColor(size_t row, size_t num, board::Color color) {
        if (getColor(row, num) != Color::empty)
            decColorCount(getColor(row, num));
        if (color != Color::empty)
            incColorCount(color);

        mBoard[row][num] = color;
    }

    void Board::incColorCount(board::Color color) {
        switch (color) {
            case Color::red:
                ++mRedCellsCount;
                return;
            case Color::blue:
                ++mBlueCellsCount;
                return;
            default:
                throw std::invalid_argument("Try to increment count of not allowed type of cell!");
        }
    }

    void Board::decColorCount(board::Color color) {
        switch (color) {
            case Color::red:
                --mRedCellsCount;
                return;
            case Color::blue:
                --mBlueCellsCount;
                return;
            default:
                throw std::invalid_argument("Try to decrement count of not allowed type of cell!");
        }
    }

    void Board::fillEmptyCellsForPlayer(data_structures::List<CellCoords> &cellList,
                                        const DistancesType &playerDistances1,
                                        const DistancesType &playerDistances2,
                                        size_t maxDistance) {
        bool canAddIdleCell = maxDistance > 1;
        for (size_t row = 0u; row < size(); ++row) {
            for (size_t num = 0u; num < size(); ++num) {
                if (mBoard[row][num] == Color::empty) {
                    if (canBeWinningCell(playerDistances1, playerDistances2, maxDistance, row, num)) {
                        cellList.pushBack({row, num});
                        continue;
                    }
                    if (canAddIdleCell) {
                        cellList.pushBack({row, num});
                        canAddIdleCell = false;
                    }
                }
            }
        }
    }

    void Board::fillEmptyCellsForOpponent(data_structures::List<CellCoords> &cellList,
                                          const DistancesType &playerDistances1,
                                          const DistancesType &playerDistances2,
                                          const DistancesType &opponentDistances1,
                                          const DistancesType &opponentDistances2,
                                          size_t playerMaxDistance,
                                          size_t opponentMaxDistance) {
        bool canAddIdleCell = playerMaxDistance > 1;
        for (size_t row = size(); row > 0;) { // go in opposite direction than for player
            --row; // not done in loop definition to avoid casting value from int to size_t
            for (size_t num = size(); num > 0;) {
                --num; // not done in loop definition - same reason as for row
                if (mBoard[row][num] == Color::empty) {
                    if (canBeWinningCell(playerDistances1, playerDistances2, playerMaxDistance, row, num) ||
                        canBeWinningCell(opponentDistances1, opponentDistances2, opponentMaxDistance, row, num)) {
                        cellList.pushBack({row, num});
                        continue;
                    }
                    if (canAddIdleCell) {
                        cellList.pushBack({row, num});
                        canAddIdleCell = false;
                    }
                }
            }
        }
    }


} // board