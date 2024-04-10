//
// Created by stefan on 09/04/24.
//

#include "gtest/gtest.h"

#include "Bfs.h"
#include "Board.h"
#include "NeighboursHelpers.h"
#include "DoneVerifiers.h"
#include "CellCoords.h"

using board::algorithms::Bfs;
using board::Board;
using board::CellCoords;
using board::RedStraightNeighbourHelper;
using board::BlueStraightNeighbourHelper;
using board::RedDoneVerifier;
using board::BlueDoneVerifier;
using data_structures::Vector;

class BfsTest : public ::testing::Test {
protected:
    typedef Bfs::DistancesType DistancesType;

    void initDistances(DistancesType &distances) {
        for (auto &row: distances) {
            row.init(distances.size());
            std::fill_n(row.begin(), distances.size(), SIZE_MAX);
        }
    }

    void verifyDistances(const std::vector<std::vector<size_t>> &expectedDistances,
                         const DistancesType &testedDistances, size_t boardSize, const std::string &name) {
        for (size_t row = 0u; row < boardSize; ++row) {
            for (size_t num = 0; num < boardSize; ++num) {
                ASSERT_EQ(expectedDistances[row][num], testedDistances[row][num])
                                            << name << ": Wrong distance at position (" << row << ", " << num << ")";
            }
        }
    }

    void setColor(Board &board, const std::vector<std::pair<size_t, size_t >> &coordinates, board::Cell::Type color) {
        for (const auto &coords: coordinates) {
            board.setType(coords.first, coords.second, color);
        }
    }

    List<CellCoords *> getStartCoordsForRed(const Board &board) {
        List<CellCoords *> list;
        for (size_t row = 0u; row < board.size(); ++row) {
            if (board.getType(row, 0) == board::Cell::Type::red) {
                list.pushBack(new CellCoords{row, 0});
            }
        }
        return list;
    }

    List<CellCoords *> getStartCoordsForBlue(const Board &board) {
        List<CellCoords *> list;
        for (size_t num = 0u; num < board.size(); ++num) {
            if (board.getType(0, num) == board::Cell::Type::blue) {
                list.pushBack(new CellCoords{0, num});
            }
        }
        return list;
    }
};


TEST_F(BfsTest, fillDistancesForEmptyBoard) {
    size_t boardSize = 3;
    Board board(boardSize);
    const std::vector<std::vector<size_t>> expectedLeftBorderDistances = {
            {1, 2, 3},
            {1, 2, 3},
            {1, 2, 3},
    };
    const std::vector<std::vector<size_t>> expectedRightBorderDistances = {
            {3, 2, 1},
            {3, 2, 1},
            {3, 2, 1},
    };
    const std::vector<std::vector<size_t>> expectedTopBorderDistances = {
            {1, 1, 1},
            {2, 2, 2},
            {3, 3, 3},
    };
    const std::vector<std::vector<size_t>> expectedBottomBorderDistances = {
            {3, 3, 3},
            {2, 2, 2},
            {1, 1, 1},
    };

    DistancesType distancesToLeftBorder(boardSize);
    DistancesType distancesToRightBorder(boardSize);
    DistancesType distancesToTopBorder(boardSize);
    DistancesType distancesToBottomBorder(boardSize);

    Bfs::fillDistancesForEmptyCells(board, distancesToLeftBorder, distancesToRightBorder, distancesToTopBorder,
                                    distancesToBottomBorder);

    verifyDistances(expectedLeftBorderDistances, distancesToLeftBorder, boardSize, "Left");
    verifyDistances(expectedRightBorderDistances, distancesToRightBorder, boardSize, "Right");
    verifyDistances(expectedTopBorderDistances, distancesToTopBorder, boardSize, "Top");
    verifyDistances(expectedBottomBorderDistances, distancesToBottomBorder, boardSize, "Bottom");
}

TEST_F(BfsTest, fillDistances) {
    size_t boardSize = 6;
    Board board(boardSize);

    const std::vector<std::pair<size_t, size_t>> redCoordinates = {{0, 0},
                                                                   {0, 3},
                                                                   {1, 1},
                                                                   {1, 4},
                                                                   {2, 2},
                                                                   {4, 0},
                                                                   {4, 5},
                                                                   {5, 1},
                                                                   {5, 2},};
    const std::vector<std::pair<size_t, size_t>> blueCoordinates = {{0, 4},
                                                                    {1, 5},};
    setColor(board, redCoordinates, board::Cell::Type::red);
    setColor(board, blueCoordinates, board::Cell::Type::blue);

    const std::vector<std::vector<size_t>> expectedLeftBorderDistances = {
            {0, 1, 2, 2, SIZE_MAX, SIZE_MAX},
            {1, 0, 1, 2, 2,        SIZE_MAX},
            {1, 1, 0, 1, 2, 3},
            {1, 2, 1, 1, 2, 3},
            {0, 1, 1, 2, 2, 2},
            {1, 0, 0, 1, 2, 3},
    };
    const std::vector<std::vector<size_t>> expectedRightBorderDistances = {
            {2, 3, 2, 1, SIZE_MAX, 1},
            {3, 2, 3, 2, 1, SIZE_MAX},
            {4, 3, 2, 2, 2,        1},
            {4, 4, 3, 2, 1,        1},
            {3, 4, 3, 2, 1,        0},
            {4, 3, 3, 3, 2,        1},
    };
    const std::vector<std::vector<size_t>> expectedTopBorderDistances = {
            {SIZE_MAX, 1,        1, SIZE_MAX, 0,        1},
            {7,        SIZE_MAX, 2,        2, SIZE_MAX, 0},
            {7,        6,        SIZE_MAX, 3, 2,        1},
            {7,        6,        5,        4, 3,        2},
            {SIZE_MAX, 7,        6,        5, 4, SIZE_MAX},
            {SIZE_MAX, SIZE_MAX, SIZE_MAX, 6, 5,        5},
    };
    const std::vector<std::vector<size_t>> expectedBottomBorderDistances = {
            {SIZE_MAX, 6, 6, SIZE_MAX, 5,        6},
            {5, SIZE_MAX, 5,        5, SIZE_MAX, 5},
            {4,        4, SIZE_MAX, 4, 4,        5},
            {4,        3, 3,        3, 3,        4},
            {SIZE_MAX, 3, 2,        2, 2, SIZE_MAX},
            {1, SIZE_MAX, SIZE_MAX, 1, 1,        1},
    };

    DistancesType distancesToLeftBorder(boardSize);
    DistancesType distancesToRightBorder(boardSize);
    DistancesType distancesToTopBorder(boardSize);
    DistancesType distancesToBottomBorder(boardSize);

    Bfs::fillDistancesForEmptyCells(board, distancesToLeftBorder, distancesToRightBorder, distancesToTopBorder,
                                    distancesToBottomBorder);

    verifyDistances(expectedLeftBorderDistances, distancesToLeftBorder, boardSize, "Left");
    verifyDistances(expectedRightBorderDistances, distancesToRightBorder, boardSize, "Right");
    verifyDistances(expectedTopBorderDistances, distancesToTopBorder, boardSize, "Top");
    verifyDistances(expectedBottomBorderDistances, distancesToBottomBorder, boardSize, "Bottom");
}

TEST_F(BfsTest, bfsNoWinner) {
    size_t boardSize = 6;
    Board board(boardSize);

    const std::vector<std::pair<size_t, size_t>> redCoordinates = {{0, 0},
                                                                   {0, 3},
                                                                   {1, 1},
                                                                   {1, 4},
                                                                   {2, 2},
                                                                   {4, 0},
                                                                   {4, 5},
                                                                   {5, 1},
                                                                   {5, 2},};
    const std::vector<std::pair<size_t, size_t>> blueCoordinates = {{0, 4},
                                                                    {1, 0},
                                                                    {1, 5},
                                                                    {2, 1},
                                                                    {2, 4},
                                                                    {3, 1},
                                                                    {3, 2},
                                                                    {4, 3},
                                                                    {4, 4},
                                                                    {5, 3},
                                                                    {5, 5},};
    setColor(board, redCoordinates, board::Cell::Type::red);
    setColor(board, blueCoordinates, board::Cell::Type::blue);

    RedStraightNeighbourHelper redStraightNeighbourHelper(board);
    BlueStraightNeighbourHelper blueStraightNeighbourHelper(board);
    RedDoneVerifier redDoneVerifier(board);
    BlueDoneVerifier blueDoneVerifier(board);
    List<CellCoords *> redNexts = getStartCoordsForRed(board);
    List<CellCoords *> blueNexts = getStartCoordsForBlue(board);

    ASSERT_FALSE(Bfs(board, redStraightNeighbourHelper, redDoneVerifier)(redNexts));
    ASSERT_FALSE(Bfs(board, blueStraightNeighbourHelper, blueDoneVerifier)(blueNexts));
}

TEST_F(BfsTest, bfsRedWin) {
    size_t boardSize = 6;
    Board board(boardSize);

    const std::vector<std::pair<size_t, size_t>> redCoordinates = {{0, 0},
                                                                   {0, 3},
                                                                   {1, 1},
                                                                   {1, 4},
                                                                   {2, 2},
                                                                   {4, 0},
                                                                   {4, 5},
                                                                   {5, 1},
                                                                   {5, 2},
                                                                   {2, 3},
                                                                   {3, 4},};
    const std::vector<std::pair<size_t, size_t>> blueCoordinates = {{0, 4},
                                                                    {1, 0},
                                                                    {1, 5},
                                                                    {2, 1},
                                                                    {2, 4},
                                                                    {3, 1},
                                                                    {3, 2},
                                                                    {4, 3},
                                                                    {4, 4},
                                                                    {5, 3},
                                                                    {5, 5},};
    setColor(board, redCoordinates, board::Cell::Type::red);
    setColor(board, blueCoordinates, board::Cell::Type::blue);

    RedStraightNeighbourHelper redStraightNeighbourHelper(board);
    BlueStraightNeighbourHelper blueStraightNeighbourHelper(board);
    RedDoneVerifier redDoneVerifier(board);
    BlueDoneVerifier blueDoneVerifier(board);
    List<CellCoords *> redNexts = getStartCoordsForRed(board);
    List<CellCoords *> blueNexts = getStartCoordsForBlue(board);

    ASSERT_TRUE(Bfs(board, redStraightNeighbourHelper, redDoneVerifier)(redNexts));
    ASSERT_FALSE(Bfs(board, blueStraightNeighbourHelper, blueDoneVerifier)(blueNexts));
}

TEST_F(BfsTest, bfsBlueWin) {
    size_t boardSize = 6;
    Board board(boardSize);

    const std::vector<std::pair<size_t, size_t>> redCoordinates = {{0, 0},
                                                                   {0, 3},
                                                                   {1, 1},
                                                                   {1, 4},
                                                                   {2, 2},
                                                                   {4, 0},
                                                                   {4, 5},
                                                                   {5, 1},
                                                                   {5, 2},};
    const std::vector<std::pair<size_t, size_t>> blueCoordinates = {{0, 4},
                                                                    {1, 0},
                                                                    {1, 5},
                                                                    {2, 1},
                                                                    {2, 4},
                                                                    {3, 1},
                                                                    {3, 2},
                                                                    {4, 3},
                                                                    {4, 4},
                                                                    {5, 3},
                                                                    {5, 5},
                                                                    {2, 5},
                                                                    {3, 4},};
    setColor(board, redCoordinates, board::Cell::Type::red);
    setColor(board, blueCoordinates, board::Cell::Type::blue);

    RedStraightNeighbourHelper redStraightNeighbourHelper(board);
    BlueStraightNeighbourHelper blueStraightNeighbourHelper(board);
    RedDoneVerifier redDoneVerifier(board);
    BlueDoneVerifier blueDoneVerifier(board);
    List<CellCoords *> redNexts = getStartCoordsForRed(board);
    List<CellCoords *> blueNexts = getStartCoordsForBlue(board);

    ASSERT_FALSE(Bfs(board, redStraightNeighbourHelper, redDoneVerifier)(redNexts));
    ASSERT_TRUE(Bfs(board, blueStraightNeighbourHelper, blueDoneVerifier)(blueNexts));
}
