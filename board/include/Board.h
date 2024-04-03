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
        typedef Cell::Type CellType;

        explicit Board(size_t mSize);

        Board(const Board &other);

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

        void setType(size_t row, size_t cellNum, CellType type);

        size_t getColorCount(Cell::Type color) const;

        bool isBoardCorrect() const;

        bool isBoardPossible() const;

        bool isRedWin() const;

        bool isBlueWin() const;

    private:
        bool bfs(Cell *start, Cell *end, CellType acceptedType);

        void incColorCount(Cell::Type color);

        void decColorCount(Cell::Type color);

        inline static void createConnection(Cell *c1, Cell *c2);

        void generateCellsAndConnections();

        size_t mSize;
        size_t redCellsCount;
        size_t blueCellsCount;
        Cell mRedBoarderLeft;
        Cell mRedBoarderRight;
        Cell mBlueBoarderLeft;
        Cell mBlueBoarderRight;
        data_structures::Vector<data_structures::Vector<Cell *> *> mBoard;
    };

} // board

#endif //HEX_BOARD_H
