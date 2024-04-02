//
// Created by stefan on 02/04/24.
//

#include "HexVerifier.h"

#include "Board.h"
#include "BoardParser.h"
#include "String.h"

using utils::String;

namespace board {

    void HexVerifier::handleQuery(utils::String *query, Board *board) {
        if (*query == "BOARD_SIZE") {
            printf("%lu\n", board->getSize());
            return;
        }
        printf("%s: query is not handled yet\n", query->c_str());
    }

    void HexVerifier::handleBoard(utils::InputReader &inputReader) {
        BoardParser boardParser(inputReader);
        Board *board = boardParser.generateBoard();
        if (board == nullptr)
            return;

        String *lastLine = inputReader.getLine();
        while (lastLine->size() > 0) {
            handleQuery(lastLine, board);
            delete lastLine;
            lastLine = inputReader.getLine();
        }
        delete lastLine;
        delete board;
    }


    void HexVerifier::handleInput(utils::InputReader &inputReader) {
        while(!inputReader.eof()){
            handleBoard(inputReader);
        }
    }

} // board