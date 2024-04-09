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
            {Cell::Type::empty, Cell::Type::empty, Cell::Type::empty, Cell::Type::red},
            {Cell::Type::blue,  Cell::Type::red,   Cell::Type::empty, Cell::Type::blue},
            {Cell::Type::red,   Cell::Type::blue,  Cell::Type::red,   Cell::Type::red},
            {Cell::Type::blue,  Cell::Type::empty, Cell::Type::empty, Cell::Type::empty},
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

    ASSERT_EQ(4, board->size());
    ASSERT_EQ(4, board->getColorCount(Cell::Type::blue));
    ASSERT_EQ(5, board->getColorCount(Cell::Type::red));
    ASSERT_EQ(7, board->getColorCount(Cell::Type::empty));

    for (size_t row = 0u; row < expectedTypes.size(); ++row) {
        for (size_t num = 0u; num < expectedTypes[row].size(); ++num) {
            ASSERT_EQ(expectedTypes[row][num], board->getType(row, num))
                                        << "Wrong type at position (" << row << ", " << num << ")";
        }
    }
}

TEST_F(BoardParserTest, boardSize7Parse) {
    const std::vector<std::vector<Cell::Type>> expectedTypes = {
            {Cell::Type::empty, Cell::Type::red,   Cell::Type::empty, Cell::Type::red,   Cell::Type::empty, Cell::Type::red,   Cell::Type::blue},
            {Cell::Type::blue,  Cell::Type::empty, Cell::Type::empty, Cell::Type::empty, Cell::Type::red,   Cell::Type::red,   Cell::Type::empty},
            {Cell::Type::empty, Cell::Type::blue,  Cell::Type::empty, Cell::Type::red,   Cell::Type::empty, Cell::Type::red,   Cell::Type::red},
            {Cell::Type::empty, Cell::Type::red,   Cell::Type::blue,  Cell::Type::red,   Cell::Type::empty, Cell::Type::empty, Cell::Type::empty},
            {Cell::Type::empty, Cell::Type::empty, Cell::Type::empty, Cell::Type::empty, Cell::Type::blue,  Cell::Type::red,   Cell::Type::empty},
            {Cell::Type::blue,  Cell::Type::empty, Cell::Type::blue,  Cell::Type::red,   Cell::Type::empty, Cell::Type::empty, Cell::Type::empty},
            {Cell::Type::empty, Cell::Type::empty, Cell::Type::empty, Cell::Type::blue,  Cell::Type::red,   Cell::Type::blue,  Cell::Type::empty},
    };
    const std::list<String *> inputLines = {
            new String("---"),
            new String("--<   >--"),
            new String("--< b >-< r >--"),
            new String("--<   >-<   >-<   >--"),
            new String("--<   >-< b >-<   >-< r >--"),
            new String("--<   >-< r >-<   >-<   >-<   >--"),
            new String("--< b >-<   >-< b >-< r >-< r >-< r >--"),
            new String("<   >-<   >-<   >-< r >-<   >-< r >-< b >"),
            new String("--<   >-< b >-<   >-<   >-< r >-<   >--"),
            new String("--<   >-< r >-< b >-<   >-< r >--"),
            new String("--< b >-<   >-< r >-<   >--"),
            new String("--< r >-<   >-<   >--"),
            new String("--< b >-<   >--"),
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

    ASSERT_EQ(7, board->size());
    ASSERT_EQ(9, board->getColorCount(Cell::Type::blue));
    ASSERT_EQ(13, board->getColorCount(Cell::Type::red));
    ASSERT_EQ(27, board->getColorCount(Cell::Type::empty));

    for (size_t row = 0u; row < expectedTypes.size(); ++row) {
        for (size_t num = 0u; num < expectedTypes[row].size(); ++num) {
            ASSERT_EQ(expectedTypes[row][num], board->getType(row, num))
                                        << "Wrong type at position (" << row << ", " << num << ")";
        }
    }
}
