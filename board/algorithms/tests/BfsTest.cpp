//
// Created by stefan on 09/04/24.
//

#include "gtest/gtest.h"

#include "Bfs.h"
#include "Board.h"

using board::algorithms::Bfs;
using board::Board;
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

};


TEST_F(BfsTest, fillDistancesForEmptyBoard) {
    size_t boardSize = 3;
    Board board(boardSize);

    DistancesType distancesToLeftBorder(boardSize);
    DistancesType distancesToRightBorder(boardSize);
    DistancesType distancesToTopBorder(boardSize);
    DistancesType distancesToBottomBorder(boardSize);


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

    Bfs bfs(board);
    bfs.fillDistancesForEmptyCells(
            distancesToLeftBorder, distancesToRightBorder, distancesToTopBorder, distancesToBottomBorder);

    verifyDistances(expectedLeftBorderDistances, distancesToLeftBorder, boardSize, "Left");
    verifyDistances(expectedRightBorderDistances, distancesToRightBorder, boardSize, "Right");
    verifyDistances(expectedTopBorderDistances, distancesToTopBorder, boardSize, "Top");
    verifyDistances(expectedBottomBorderDistances, distancesToBottomBorder, boardSize, "Bottom");
}

TEST_F(BfsTest, fillDistances) {
    size_t boardSize = 6;
    Board board(boardSize);

    const std::vector<std::pair<size_t, size_t>> redCoordinates = {
            {0, 0},
            {0, 3},
            {1, 1},
            {1, 4},
            {2, 2},
            {4, 0},
            {4, 5},
            {5, 1},
            {5, 2},
    };
    const std::vector<std::pair<size_t, size_t>> blueCoordinates = {
            {0, 4},
            {1, 5},
    };
    for (const auto &coords: redCoordinates) {
        board.setType(coords.first, coords.second, board::Cell::Type::red);
    }
    for (const auto &coords: blueCoordinates) {
        board.setType(coords.first, coords.second, board::Cell::Type::blue);
    }

    DistancesType distancesToLeftBorder(boardSize);
    DistancesType distancesToRightBorder(boardSize);
    DistancesType distancesToTopBorder(boardSize);
    DistancesType distancesToBottomBorder(boardSize);

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

    Bfs bfs(board);
    bfs.fillDistancesForEmptyCells(
            distancesToLeftBorder, distancesToRightBorder, distancesToTopBorder, distancesToBottomBorder);

    verifyDistances(expectedLeftBorderDistances, distancesToLeftBorder, boardSize, "Left");
    verifyDistances(expectedRightBorderDistances, distancesToRightBorder, boardSize, "Right");
    verifyDistances(expectedTopBorderDistances, distancesToTopBorder, boardSize, "Top");
    verifyDistances(expectedBottomBorderDistances, distancesToBottomBorder, boardSize, "Bottom");
}