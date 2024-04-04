//
// Created by stefan on 01/04/24.
//

#ifndef HEX_BOARD_H
#define HEX_BOARD_H

#include "Cell.h"
#include "Vector.h"

#include <functional>

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

        Cell *getCell(size_t row, size_t cellNum) const {
            return mBoard[row]->at(cellNum);
        }

        const Cell *getLastCellInRow(size_t row) const {
            return mBoard[row]->last();
        }

        const Cell &getBoarder(Cell::Type type) const;

        void setType(size_t row, size_t cellNum, CellType type);

        size_t getColorCount(Cell::Type color) const;

        bool isBoardCorrect() const;

        bool isBoardPossible();

        bool isRedWin();

        bool isBlueWin();

        bool canRedWinInNMoves(size_t n);

        bool canBlueWinInNMoves(size_t n);

    private:
        struct CellParent {
            Cell *cell;
            Cell *parent;
        };
        struct CellCoords {
            enum class Direction {
                up,
                down,
                left,
                right,
            };
            size_t row;
            size_t num;
            Cell *parent;
            Direction direction;
        };

        bool bfs(Cell *start, Cell *end, CellType acceptedType);

        bool bfs(data_structures::List<CellCoords> &nexts,
                 CellType color,
                 Cell &endBorder,
                 const std::function<bool(const Board &, size_t, size_t)> &done,
                 const std::function<data_structures::List<CellCoords>(const Board &, size_t, size_t)> &getNeighbours);

        bool dfs(const data_structures::List<CellCoords> &nexts,
                 CellType color,
                 Cell &endBorder,
                 const std::function<bool(const Board &, size_t, size_t)> &done,
                 const std::function<data_structures::List<CellCoords>(const Board &,
                                                                       const CellCoords &cellCoords)> &getNeighbours);

        bool dfs(const CellCoords &cellCoords,
                 CellType color,
                 Cell &endBorder,
                 const std::function<bool(const Board &, size_t, size_t)> &done,
                 const std::function<data_structures::List<CellCoords>(const Board &,
                                                                       const CellCoords &cellCoords)> &getNeighbours);

        static bool doneForBlue(const Board &board, size_t row, size_t num);

        static bool doneForRed(const Board &board, size_t row, size_t num);

        static data_structures::List<CellCoords> getNeighboursForBlue(const Board &board, const CellCoords &cellCoords);

        static data_structures::List<CellCoords> getNeighboursForRed(const Board &board, const CellCoords &cellCoords);

        void addNeighbourAbove(size_t row, size_t num, data_structures::List<CellCoords> &neighbours) const;

        void addNeighbourBelow(size_t row, size_t num, data_structures::List<CellCoords> &neighbours) const;

        void
        addNeighbourOnLeft(size_t row, size_t num, data_structures::List<CellCoords> &neighbours, bool topFirst) const;

        void
        addNeighbourOnRight(size_t row, size_t num, data_structures::List<CellCoords> &neighbours, bool topFirst) const;

        void incColorCount(Cell::Type color);

        void decColorCount(Cell::Type color);

        inline static void createConnection(Cell *c1, Cell *c2);

        void generateCellsAndConnections();

        void clearVisited();

        void visitParents(const Cell *lastChild, const Cell *greatParent);

        bool canWin(size_t movesLeft, CellType color, data_structures::List<CellCoords> &emptyCellsCoords,
                    const std::function<bool(Board &)> &isWin);

        static bool isRedWinBfs(Board &board) {
            return board.bfs(&board.mRedBoarderLeft, &board.mRedBoarderRight, CellType::red);
        }

        static bool isBlueWinBfs(Board &board) {
            return board.bfs(&board.mBlueBoarderRight, &board.mBlueBoarderLeft, Cell::Type::blue);
        }

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
