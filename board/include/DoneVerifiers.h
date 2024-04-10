//
// Created by stefan on 08/04/24.
//

#ifndef HEX_DONEVERIFIERS_H
#define HEX_DONEVERIFIERS_H

#include "CellCoords.h"

namespace board {
    class Board;

    class DoneVerifier {
    public:
        DoneVerifier() = default;

        DoneVerifier(const DoneVerifier &) = delete;

        virtual bool operator()(const CellCoords *cellCoords) const = 0;
    };

    class RedDoneVerifier : public DoneVerifier {
    public:
        explicit RedDoneVerifier(const Board &board) : mBoard(board) {}

        bool operator()(const CellCoords *cellCoords) const override;

    private:
        const Board &mBoard;
    };

    class BlueDoneVerifier : public DoneVerifier {
    public:
        explicit BlueDoneVerifier(const Board &board) : mBoard(board) {}

        bool operator()(const CellCoords *cellCoords) const override;

    private:
        const Board &mBoard;
    };

} // board

#endif //HEX_DONEVERIFIERS_H
