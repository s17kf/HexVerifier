//
// Created by stefan on 01/04/24.
//

#ifndef HEX_BOARD_H
#define HEX_BOARD_H

#include "Cell.h"
#include "Vector.h"

namespace board {

    class Board {
    public:
        typedef data_structures::Vector<Cell *> RowType;

        explicit Board(size_t mSize);

        virtual ~Board();

        size_t getSize() const {
            return mSize;
        }

        const Cell *getCell(size_t row, size_t cellNum) const {
            return mBoard[row]->at(cellNum);
        }

        const Cell *getLastCellInRow(size_t row) const {
            return mBoard[row]->last();
        }

        const Cell &getBoarder(Cell::Type type) const;

    private:
        inline static void createConnection(Cell* c1, Cell* c2);

        size_t mSize;
        Cell mRedBoarderLeft;
        Cell mRedBoarderRight;
        Cell mBlueBoarderLeft;
        Cell mBlueBoarderRight;
        data_structures::Vector<data_structures::Vector<Cell *> *> mBoard;
    };

} // board

#endif //HEX_BOARD_H
