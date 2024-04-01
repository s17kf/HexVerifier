//
// Created by stefan on 29/03/24.
//

#include "gtest/gtest.h"

#include "Board.h"
#include "Cell.h"

using board::Board;
using board::Cell;

class BoardTest : public ::testing::Test {
protected:
    const std::vector<Cell::Type> BOARDER_TYPES = {
            Cell::Type::redBorderRight,
            Cell::Type::redBorderLeft,
            Cell::Type::blueBorderRight,
            Cell::Type::blueBorderLeft,
    };

    void verifyBoardersNeighbours(const Board &board) {
        const auto boardSize = board.getSize();
        for (auto borderType: BOARDER_TYPES) {
            ASSERT_EQ(board.getSize(), board.getBoarder(borderType).getNeighbours().size());
            const auto &boarderNeighbours = board.getBoarder(borderType).getNeighbours();
            switch (borderType) {
                case Cell::Type::redBorderRight: {
                    for (auto row = boardSize - 1; row < 2 * boardSize - 1; ++row) {
                        const auto *cell = board.getLastCellInRow(row);
                        ASSERT_NE(boarderNeighbours.end(),
                                  std::find(boarderNeighbours.begin(), boarderNeighbours.end(), cell));
                        ASSERT_NE(cell->getNeighbours().end(),
                                  std::find_if(cell->getNeighbours().begin(), cell->getNeighbours().end(),
                                               [&borderType](const auto *c) { return c->getType() == borderType; }));
                    }
                    break;
                }
                case Cell::Type::redBorderLeft: {
                    for (auto row = 0u; row < boardSize; ++row) {
                        const auto *cell = board.getCell(row, 0);
                        ASSERT_NE(boarderNeighbours.end(),
                                  std::find(boarderNeighbours.begin(), boarderNeighbours.end(), cell));
                        ASSERT_NE(cell->getNeighbours().end(),
                                  std::find_if(cell->getNeighbours().begin(), cell->getNeighbours().end(),
                                               [&borderType](const auto *c) { return c->getType() == borderType; }));
                    }
                    break;
                }
                case Cell::Type::blueBorderRight: {
                    for (auto row = 0u; row < board.getSize(); ++row) {
                        const auto *cell = board.getLastCellInRow(row);
                        ASSERT_NE(boarderNeighbours.end(),
                                  std::find(boarderNeighbours.begin(), boarderNeighbours.end(), cell));
                        ASSERT_NE(cell->getNeighbours().end(),
                                  std::find_if(cell->getNeighbours().begin(), cell->getNeighbours().end(),
                                               [&borderType](const auto *c) { return c->getType() == borderType; }));
                    }
                    break;
                }
                case Cell::Type::blueBorderLeft: {
                    for (auto row = boardSize - 1; row < 2 * boardSize - 1; ++row) {
                        const auto *cell = board.getCell(row, 0);
                        ASSERT_NE(boarderNeighbours.end(),
                                  std::find(boarderNeighbours.begin(), boarderNeighbours.end(), cell));
                        ASSERT_NE(cell->getNeighbours().end(),
                                  std::find_if(cell->getNeighbours().begin(), cell->getNeighbours().end(),
                                               [&borderType](const auto *c) { return c->getType() == borderType; }));
                    }
                    break;
                }
            }
        }
    }

};

TEST_F(BoardTest, createOneSizeBoard) {
    const size_t boardSize = 1;
    Board board(boardSize);

    ASSERT_EQ(boardSize, board.getSize());
    verifyBoardersNeighbours(board);
}


TEST_F(BoardTest, createFourSizeBoard) {
    const size_t boardSize = 4;
    Board board(boardSize);

    ASSERT_EQ(boardSize, board.getSize());
    verifyBoardersNeighbours(board);
    {
        const auto *cell21 = board.getCell(2, 1);
        ASSERT_EQ(6, cell21->getNeighbours().size());
        ASSERT_NE(cell21->getNeighbours().end(),
                  std::find(cell21->getNeighbours().begin(), cell21->getNeighbours().end(), board.getCell(0, 0)));
        ASSERT_NE(cell21->getNeighbours().end(),
                  std::find(cell21->getNeighbours().begin(), cell21->getNeighbours().end(), board.getCell(1, 0)));
        ASSERT_NE(cell21->getNeighbours().end(),
                  std::find(cell21->getNeighbours().begin(), cell21->getNeighbours().end(), board.getCell(1, 1)));
        ASSERT_NE(cell21->getNeighbours().end(),
                  std::find(cell21->getNeighbours().begin(), cell21->getNeighbours().end(), board.getCell(3, 1)));
        ASSERT_NE(cell21->getNeighbours().end(),
                  std::find(cell21->getNeighbours().begin(), cell21->getNeighbours().end(), board.getCell(3, 2)));
        ASSERT_NE(cell21->getNeighbours().end(),
                  std::find(cell21->getNeighbours().begin(), cell21->getNeighbours().end(), board.getCell(4, 1)));
    }

    {
        const auto *cell51 = board.getCell(5, 1);
        ASSERT_EQ(5, cell51->getNeighbours().size());
        ASSERT_NE(cell51->getNeighbours().end(),
                  std::find(cell51->getNeighbours().begin(), cell51->getNeighbours().end(),
                            &board.getBoarder(board::Cell::Type::redBorderRight)));
        ASSERT_NE(cell51->getNeighbours().end(),
                  std::find(cell51->getNeighbours().begin(), cell51->getNeighbours().end(), board.getCell(3, 2)));
        ASSERT_NE(cell51->getNeighbours().end(),
                  std::find(cell51->getNeighbours().begin(), cell51->getNeighbours().end(), board.getCell(4, 1)));
        ASSERT_NE(cell51->getNeighbours().end(),
                  std::find(cell51->getNeighbours().begin(), cell51->getNeighbours().end(), board.getCell(4, 2)));
        ASSERT_NE(cell51->getNeighbours().end(),
                  std::find(cell51->getNeighbours().begin(), cell51->getNeighbours().end(), board.getCell(6, 0)));
    }

    {
        const auto *cell40 = board.getCell(4, 0);
        ASSERT_EQ(5, cell40->getNeighbours().size());
        ASSERT_NE(cell40->getNeighbours().end(),
                  std::find(cell40->getNeighbours().begin(), cell40->getNeighbours().end(),
                            &board.getBoarder(board::Cell::Type::blueBorderLeft)));
        ASSERT_NE(cell40->getNeighbours().end(),
                  std::find(cell40->getNeighbours().begin(), cell40->getNeighbours().end(), board.getCell(2, 0)));
        ASSERT_NE(cell40->getNeighbours().end(),
                  std::find(cell40->getNeighbours().begin(), cell40->getNeighbours().end(), board.getCell(3, 0)));
        ASSERT_NE(cell40->getNeighbours().end(),
                  std::find(cell40->getNeighbours().begin(), cell40->getNeighbours().end(), board.getCell(3, 1)));
        ASSERT_NE(cell40->getNeighbours().end(),
                  std::find(cell40->getNeighbours().begin(), cell40->getNeighbours().end(), board.getCell(5, 0)));
    }

    {
        const auto *cell22 = board.getCell(2, 2);
        ASSERT_EQ(5, cell22->getNeighbours().size());
        ASSERT_NE(cell22->getNeighbours().end(),
                  std::find(cell22->getNeighbours().begin(), cell22->getNeighbours().end(),
                            &board.getBoarder(board::Cell::Type::blueBorderRight)));
        ASSERT_NE(cell22->getNeighbours().end(),
                  std::find(cell22->getNeighbours().begin(), cell22->getNeighbours().end(), board.getCell(4, 2)));
        ASSERT_NE(cell22->getNeighbours().end(),
                  std::find(cell22->getNeighbours().begin(), cell22->getNeighbours().end(), board.getCell(1, 1)));
        ASSERT_NE(cell22->getNeighbours().end(),
                  std::find(cell22->getNeighbours().begin(), cell22->getNeighbours().end(), board.getCell(3, 2)));
        ASSERT_NE(cell22->getNeighbours().end(),
                  std::find(cell22->getNeighbours().begin(), cell22->getNeighbours().end(), board.getCell(3, 3)));
    }
    
    {
        const auto *cell00 = board.getCell(0, 0);
        ASSERT_EQ(5, cell00->getNeighbours().size());
        ASSERT_NE(cell00->getNeighbours().end(),
                  std::find(cell00->getNeighbours().begin(), cell00->getNeighbours().end(),
                            &board.getBoarder(board::Cell::Type::redBorderLeft)));
        ASSERT_NE(cell00->getNeighbours().end(),
                  std::find(cell00->getNeighbours().begin(), cell00->getNeighbours().end(),
                            &board.getBoarder(board::Cell::Type::blueBorderRight)));
        ASSERT_NE(cell00->getNeighbours().end(),
                  std::find(cell00->getNeighbours().begin(), cell00->getNeighbours().end(), board.getCell(2, 1)));
        ASSERT_NE(cell00->getNeighbours().end(),
                  std::find(cell00->getNeighbours().begin(), cell00->getNeighbours().end(), board.getCell(1, 0)));
        ASSERT_NE(cell00->getNeighbours().end(),
                  std::find(cell00->getNeighbours().begin(), cell00->getNeighbours().end(), board.getCell(1, 1)));
    }

}