//
// Created by stefan on 08/04/24.
//

#ifndef HEX_DONEVERIFIERS_H
#define HEX_DONEVERIFIERS_H

#include "Board.h"
#include "CellCoords.h"

namespace board {

    class DoneVerifier {
    public:
        virtual bool operator()(const CellCoords &cellCoords) const = 0;
    };

    class RedDoneVerifier : public DoneVerifier {
    public:
        explicit RedDoneVerifier(const Board &mBoard) : mBoard(mBoard) {}

        bool operator()(const CellCoords &cellCoords) const override {
            return cellCoords.row >= mBoard.getSize() - 1 && cellCoords.num == mBoard.cells(cellCoords.row) - 1;
        }

    private:
        const Board &mBoard;
    };

    class BlueDoneVerifier : public DoneVerifier {
    public:
        explicit BlueDoneVerifier(const Board &mBoard) : mBoard(mBoard) {}

        bool operator()(const CellCoords &cellCoords) const override {
            return cellCoords.row >= mBoard.getSize() - 1 && cellCoords.num == 0;
        }

    private:
       const  Board &mBoard;
    };

} // board

#endif //HEX_DONEVERIFIERS_H
