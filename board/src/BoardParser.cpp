//
// Created by stefan on 02/04/24.
//

#include "BoardParser.h"

#include "List.h"

using utils::String;
using data_structures::List;

namespace board {

    Board *BoardParser::generateBoard() {
        String *lastLine = mInputReader.getLine();
        if (*lastLine != BOARD_DELIMITER) {
            throw std::invalid_argument("Invalid input data!");
        }
        delete lastLine;
        lastLine = mInputReader.getLine();
        List<String *> lines;
        size_t linesCounter = 0;
        while (*lastLine != BOARD_DELIMITER) {
            ++linesCounter;
            lines.pushBack(lastLine);
            lastLine = mInputReader.getLine();
        }
        delete lastLine;

        auto size = (linesCounter + 1) / 2;
        auto *board = new Board(size);
        auto lineIt = lines.begin();
        for (size_t row = 0u; row < linesCounter; ++row, ++lineIt) {
            auto lineSplitted = (*lineIt)->split();
            size_t cellNum = 0u;
            for (auto &token: lineSplitted) {
                switch (token[0]) {
                    case '>':
                        ++cellNum;
                        continue;
                    case 'r':
                        board->setType(row, cellNum, Cell::Type::red);
                        continue;
                    case 'b':
                        board->setType(row, cellNum, Cell::Type::blue);
                        continue;
                }
            }
            delete *lineIt;
        }

        return board;
    }

} // board