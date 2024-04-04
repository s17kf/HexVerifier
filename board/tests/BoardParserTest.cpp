//
// Created by stefan on 02/04/24.
//
#include "gtest/gtest.h"

#include <list>

#include "BoardParser.h"
#include "Board.h"
#include "InputReaderMock.h"

using board::BoardParser;
using board::Board;
using board::Cell;
using utils::InputReaderMock;
using utils::String;

using ::testing::Sequence;
using ::testing::Return;

class BoardParserTest : public ::testing::Test {
protected:
    void SetUp() override {
        inputReaderMock = std::make_unique<InputReaderMock>();
        boardParser = std::make_unique<BoardParser>(*inputReaderMock);
    }

    std::unique_ptr<InputReaderMock> inputReaderMock;
    std::unique_ptr<BoardParser> boardParser;
};

TEST_F(BoardParserTest, boardCreationSetsProperCellTypes) {
    const std::vector<std::vector<Cell::Type>> expectedTypes = {
            {Cell::Type::empty},
            {Cell::Type::blue, Cell::Type::empty},
            {Cell::Type::red, Cell::Type::red, Cell::Type::empty},
            {Cell::Type::blue, Cell::Type::blue, Cell::Type::empty, Cell::Type::red},
            {Cell::Type::empty, Cell::Type::red, Cell::Type::blue},
            {Cell::Type::empty, Cell::Type::red},
            {Cell::Type::empty},
    };
    const std::list<String *> inputLines = {
            new String("---"),
            new String("--<   >--"),
            new String("--< b >-<   >--"),
            new String("--< r >-< r >-<   >--"),
            new String("< b >-< b >-<   >-< r >"),
            new String("--<   >-< r >-< b >--"),
            new String("--<   >-< r >--"),
            new String("--<   >--"),
            new String("---")
    };
    Sequence s1;
    for (auto *s: inputLines) {
        EXPECT_CALL(*inputReaderMock, getLine())
                .InSequence(s1)
                .WillOnce(Return(s));
    }

    std::unique_ptr<Board> board(boardParser->generateBoard());
    ASSERT_EQ(4, board->getSize());
    for (size_t rowNum = 0u; rowNum < expectedTypes.size(); ++rowNum) {
        const auto &row = expectedTypes[rowNum];
        for (size_t cellNum = 0u; cellNum < row.size(); ++cellNum) {
            ASSERT_EQ(row[cellNum], board->getCell(rowNum, cellNum)->getType());
        }
    }

    ASSERT_EQ(4, board->getColorCount(Cell::Type::blue));
    ASSERT_EQ(5, board->getColorCount(Cell::Type::red));
    ASSERT_EQ(7, board->getColorCount(Cell::Type::empty));
}
