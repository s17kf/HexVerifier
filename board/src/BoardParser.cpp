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
        if (*lastLine == BOARD_DELIMITER) {
            delete lastLine;
            lastLine = mInputReader.getLine();
        }
        List<String *> lines;
        while (*lastLine != BOARD_DELIMITER) {
            lines.pushBack(lastLine);
            lastLine = mInputReader.getLine();
        }
        delete lastLine;
        if (lines.size() == 0){
            return nullptr;
        }

        auto boardSize = (lines.size() + 1) / 2;
        auto *board = new Board(boardSize);
        auto lineIt = lines.begin();
        for (size_t line = 0u; line < lines.size(); ++line, ++lineIt) {
            auto lineSplitted = (*lineIt)->split();
            size_t tokenInLine = 0u;
            for (auto &token: lineSplitted) {
                size_t row = line < boardSize ? line - tokenInLine : boardSize - tokenInLine - 1;
                size_t num = line < boardSize ? tokenInLine : line - boardSize + tokenInLine + 1;
                switch (token[0]) {
                    case '>':
                        ++tokenInLine;
                        continue;
                    case 'r':
                        board->setType(row, num, Cell::Type::red);
                        continue;
                    case 'b':
                        board->setType(row, num, Cell::Type::blue);
                        continue;
                }
            }
            delete *lineIt;
        }

        return board;
    }

} // board