//
// Created by stefan on 01/04/24.
//

#ifndef HEX_BOARD_H
#define HEX_BOARD_H

#include "Cell.h"
#include "Vector.h"
#include "DoneVerifiers.h"
#include "NeighboursHelpers.h"
#include "DistancesKeeper.h"


namespace board {

    class Board {
    public:
        typedef data_structures::Vector<data_structures::Vector<size_t>> DistancesType;
        typedef data_structures::Vector<Cell *> RowType;
        typedef Cell::Type CellType;

        virtual ~Board() = default;

        explicit Board(size_t size);

        Board(const Board &) = delete;

        inline size_t size() const {
            return mSize;
        }

        [[nodiscard]] size_t getColorCount(Cell::Type color) const;

        [[nodiscard]] bool isBoardCorrect() const;

        [[nodiscard]] bool isGameWonByRed() const;

        [[nodiscard]] bool isGameWonByBlue() const;

        bool isBoardPossible() const;

        bool canRedWinInNMovesWithNaive(size_t n, const DistancesKeeper &distancesKeeper);

        bool canBlueWinInNMovesWithNaive(size_t n, const DistancesKeeper &distancesKeeper);

        [[nodiscard]] inline Cell::Type getType(size_t row, size_t num) const {
            return mBoard[row][num].getType();
        }

        void setType(size_t row, size_t num, CellType type);

    private:
        inline void incColorCount(Cell::Type color);

        inline void decColorCount(Cell::Type color);

        [[nodiscard]] inline bool isGameWonByRed(List<CellCoords> &path) const;

        [[nodiscard]] inline bool isGameWonByBlue(List<CellCoords> &path) const;

        [[nodiscard]] inline bool canWinInNMovesWithNaive(
                size_t n, bool movesFirst, const DistancesType &distancesToFirstBorder,
                const DistancesType &distancesToSecondBorder);

        inline void fillEmptyCells(data_structures::List<CellCoords *> &cellList);

        size_t mSize;
        size_t mRedCellsCount;
        size_t mBlueCellsCount;
        data_structures::Vector<data_structures::Vector<Cell>> mBoard;
        RedDoneVerifier mRedDoneVerifier;
        BlueDoneVerifier mBlueDoneVerifier;
        RedNeighboursHelper mRedNeighboursHelper;
        BlueNeighboursHelper mBlueNeighboursHelper;
    };

} // board

#endif //HEX_BOARD_H
