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
using board::Color;
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
    const std::vector<std::vector<Color>> expectedTypes = {
            {Color::empty, Color::empty, Color::empty, Color::red},
            {Color::blue,  Color::red,   Color::empty, Color::blue},
            {Color::red,   Color::blue,  Color::red,   Color::red},
            {Color::blue,  Color::empty, Color::empty, Color::empty},
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
    ASSERT_EQ(4, board->getColorCount(Color::blue));
    ASSERT_EQ(5, board->getColorCount(Color::red));
    ASSERT_EQ(7, board->getColorCount(Color::empty));

    for (size_t row = 0u; row < expectedTypes.size(); ++row) {
        for (size_t num = 0u; num < expectedTypes[row].size(); ++num) {
            ASSERT_EQ(expectedTypes[row][num], board->getColor(row, num))
                                        << "Wrong type at position (" << row << ", " << num << ")";
        }
    }
}

TEST_F(BoardParserTest, boardSize7Parse) {
    const std::vector<std::vector<Color>> expectedTypes = {
            {Color::empty, Color::red,   Color::empty, Color::red,   Color::empty, Color::red,   Color::blue},
            {Color::blue,  Color::empty, Color::empty, Color::empty, Color::red,   Color::red,   Color::empty},
            {Color::empty, Color::blue,  Color::empty, Color::red,   Color::empty, Color::red,   Color::red},
            {Color::empty, Color::red,   Color::blue,  Color::red,   Color::empty, Color::empty, Color::empty},
            {Color::empty, Color::empty, Color::empty, Color::empty, Color::blue,  Color::red,   Color::empty},
            {Color::blue,  Color::empty, Color::blue,  Color::red,   Color::empty, Color::empty, Color::empty},
            {Color::empty, Color::empty, Color::empty, Color::blue,  Color::red,   Color::blue,  Color::empty},
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
    ASSERT_EQ(9, board->getColorCount(Color::blue));
    ASSERT_EQ(13, board->getColorCount(Color::red));
    ASSERT_EQ(27, board->getColorCount(Color::empty));

    for (size_t row = 0u; row < expectedTypes.size(); ++row) {
        for (size_t num = 0u; num < expectedTypes[row].size(); ++num) {
            ASSERT_EQ(expectedTypes[row][num], board->getColor(row, num))
                                        << "Wrong type at position (" << row << ", " << num << ")";
        }
    }
}
