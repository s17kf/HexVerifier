//
// Created by stefan on 29/03/24.
//

#include "gtest/gtest.h"

#include "Board.h"
#include "Cell.h"

#include <algorithm>

using board::Board;
using board::Cell;

class BoardTest : public ::testing::Test {
protected:

};

TEST_F(BoardTest, createOneSizeBoard) {
    const size_t boardSize = 1;
    Board board(boardSize);

    ASSERT_EQ(boardSize, board.size());
    ASSERT_EQ(Cell::Type::empty, board.getType(0, 0));
    ASSERT_THROW(board.getType(1, 0), std::out_of_range);
    ASSERT_THROW(board.getType(0, 1), std::out_of_range);
}

TEST_F(BoardTest, createFourSizeBoard) {
    const size_t boardSize = 4;
    Board board(boardSize);

    ASSERT_EQ(boardSize, board.size());
    for (size_t row = 0u; row < boardSize; ++row) {
        for (size_t num = 0u; num < boardSize; ++num) {
            ASSERT_EQ(Cell::Type::empty, board.getType(row, num));
        }
    }
    ASSERT_THROW(board.getType(boardSize, 0), std::out_of_range);
    ASSERT_THROW(board.getType(0, boardSize), std::out_of_range);
}

TEST_F(BoardTest, fourSizeBoardWithSomeStones) {
    const size_t boardSize = 4;
    Board board(boardSize);

    const std::vector<std::pair<size_t, size_t>> redCoords = {{0, 1},
                                                              {0, 2},
                                                              {3, 3}};
    const std::vector<std::pair<size_t, size_t>> blueCoords = {{2, 1},
                                                               {2, 2},
                                                               {3, 0},
                                                               {1, 1}};

    for (const auto &coords: redCoords) {
        board.setType(coords.first, coords.second, Cell::Type::red);
    }
    for (const auto &coords: blueCoords) {
        board.setType(coords.first, coords.second, Cell::Type::blue);
    }

    ASSERT_EQ(redCoords.size(), board.getColorCount(board::Cell::Type::red));
    ASSERT_EQ(blueCoords.size(), board.getColorCount(board::Cell::Type::blue));
    ASSERT_EQ(boardSize * boardSize - redCoords.size() - blueCoords.size(),
              board.getColorCount(board::Cell::Type::empty));

    for (size_t row = 0u; row < boardSize; ++row) {
        for (size_t num = 0u; num < boardSize; ++num) {
            if (redCoords.end() != std::find_if(redCoords.begin(), redCoords.end(), [&row, &num](auto &coords) {
                return row == coords.first && num == coords.second;
            })) {
                ASSERT_EQ(Cell::Type::red, board.getType(row, num));
                continue;
            }
            if (blueCoords.end() != std::find_if(blueCoords.begin(), blueCoords.end(), [&row, &num](auto &coords) {
                return row == coords.first && num == coords.second;
            })) {
                ASSERT_EQ(Cell::Type::blue, board.getType(row, num));
                continue;
            }
            ASSERT_EQ(Cell::Type::empty, board.getType(row, num));
        }
    }
}
