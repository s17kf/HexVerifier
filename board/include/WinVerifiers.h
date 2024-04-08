//
// Created by stefan on 08/04/24.
//

#ifndef HEX_WINVERIFIERS_H
#define HEX_WINVERIFIERS_H

#include "Board.h"

namespace board {

    class WinVerifier {
    public:
        virtual bool operator()() const = 0;
    };

    class RedWinVerifier : public WinVerifier {
    public:
        explicit RedWinVerifier(Board &mBoard) : mBoard(mBoard) {}

        bool operator()() const override {
            return mBoard.bfs(&mBoard.mRedBoarderLeft, &mBoard.mRedBoarderRight, Cell::Type::red);
        }

    private:
        Board &mBoard;
    };

    class BlueWinVerifier : public WinVerifier {
    public:
        explicit BlueWinVerifier(Board &mBoard) : mBoard(mBoard) {}

        bool operator()() const override {
            return mBoard.bfs(&mBoard.mBlueBoarderRight, &mBoard.mBlueBoarderLeft, Cell::Type::blue);
        }

    private:
        Board &mBoard;
    };

} // board

#endif //HEX_WINVERIFIERS_H
