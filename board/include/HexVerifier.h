//
// Created by stefan on 02/04/24.
//

#ifndef HEX_HEXVERIFIER_H
#define HEX_HEXVERIFIER_H

#include "InputReader.h"
#include "String.h"
#include "Board.h"

namespace board {

    class HexVerifier {
    public:
        static void handleInput(utils::InputReader &inputReader);
    private:
        static void handleBoard(utils::InputReader &inputReader);
        static void handleQuery(utils::String *query, Board *board, const BoardStateUtils &boardStateUtils);
    };

} // board

#endif //HEX_HEXVERIFIER_H
