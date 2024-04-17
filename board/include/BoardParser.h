//
// Created by stefan on 02/04/24.
//

#ifndef HEX_BOARDPARSER_H
#define HEX_BOARDPARSER_H

#include "InputReader.h"
#include "Board.h"
#include "String.h"

namespace board {
    class BoardParser {
    public:
        explicit BoardParser(utils::InputReader &mInputReader) : mInputReader(mInputReader) {}

        Board *generateBoard();

        const std::string BOARD_DELIMITER = "---";
    private:
        utils::InputReader &mInputReader;
    };

} // board


#endif //HEX_BOARDPARSER_H
