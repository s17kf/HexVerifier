//
// Created by stefan on 01/04/24.
//

#ifndef HEX_BOARD_H
#define HEX_BOARD_H

#include "Cell.h"
#include "Vector.h"
#include "CellCoords.h"

#include <functional>

namespace board {

    class DoneVerifier;

    class RedDoneVerifier;

    class BlueDoneVerifier;

    class WinVerifier;

    class RedWinVerifier;

    class BlueWinVerifier;

    class NeighboursGenerator;

    class RedNeighboursGenerator;

    class BlueNeighboursGenerator;

    class BoardStateUtils;


    class Board {
    public:
        typedef data_structures::Vector<Cell *> RowType;
        typedef Cell::Type CellType;

        explicit Board(size_t mSize);

        Board(const Board &other);

        virtual ~Board();

        inline size_t getSize() const {
            return mSize;
        }

        inline size_t rows() const {
            return mBoard.size();
        }

        inline size_t cells(size_t row) const {
            return mBoard[row]->size();
        }

        inline Cell *getCell(size_t row, size_t cellNum) const {
            return mBoard[row]->at(cellNum);
        }

        inline Cell *getCell(CellCoords coords) const {
            return getCell(coords.row, coords.num);
        }

        inline Cell *getLastCellInRow(size_t row) const {
            return mBoard[row]->last();
        }

        const Cell &getBoarder(Cell::Type type) const;

        void setType(size_t row, size_t cellNum, CellType type);

        size_t getColorCount(Cell::Type color) const;

        bool isBoardCorrect() const;

        bool isBoardPossible();

        bool isWin(CellType color);

        bool isWon(CellType color, const BoardStateUtils &boardStateUtils);

        bool canRedWinInNMoves(size_t n);

        bool canBlueWinInNMoves(size_t n);

        bool canRedWinInNMovesWithPerfectOpponent(size_t n, const BoardStateUtils &boardStateUtils);

    private:
        struct CellParent {
            Cell *cell;
            Cell *parent;
        };

        enum class minMaxType {
            min,
            max
        };

        class CellDistanceEvaluator {
        public:
            virtual ~CellDistanceEvaluator() = default;

            virtual bool operator()(const Cell *cell) const = 0;
        };

        class RedCellDistanceEvaluator : public CellDistanceEvaluator {
        public:
            explicit RedCellDistanceEvaluator(size_t allowedDistance) : allowedDistance(allowedDistance) {}

            bool operator()(const Cell *cell) const override {
                return cell->closestRed < 2;
            }

        private:
            size_t allowedDistance;
        };

        class BlueCellDistanceEvaluator : public CellDistanceEvaluator {
        public:
            explicit BlueCellDistanceEvaluator(size_t allowedDistance) : allowedDistance(allowedDistance) {}

            bool operator()(const Cell *cell) const override {
                return cell->closestBlue < 2;
            }

        private:
            size_t allowedDistance;
        };

        bool bfs(Cell *start, Cell *end, CellType acceptedType);

        bool bfs(data_structures::List<CellCoords> &nexts,
                 CellType color,
                 Cell &endBorder,
                 const std::function<bool(const Board &, size_t, size_t)> &done,
                 const std::function<data_structures::List<CellCoords>(const Board &, size_t, size_t)> &getNeighbours);

        bool dfs(CellType color,
                 Cell &endBorder,
                 const DoneVerifier &doneVerifier,
                 const NeighboursGenerator &neighboursGenerator);

        bool dfs(const CellCoords &cellCoords,
                 CellType color,
                 Cell &endBorder,
                 const DoneVerifier &doneVerifier,
                 const NeighboursGenerator &neighboursGenerator);

        bool dfs(const BoardStateUtils &boardStateUtils, const DoneVerifier &doneVerifier,
                 const NeighboursGenerator &neighboursGenerator);

        bool dfs(const CellCoords &cellCoords, const BoardStateUtils &boardStateUtils, const DoneVerifier &doneVerifier,
                 const NeighboursGenerator &neighboursGenerator);

        int minMaxStateEvaluate(
                data_structures::List<CellCoords> &emptyCells, minMaxType playerType, CellType playerColor,
                size_t stepsLeft, size_t checkedMoves, const CellDistanceEvaluator &cellDistanceEvaluator,
                const BoardStateUtils &boardStateUtils);

        void incColorCount(Cell::Type color);

        void decColorCount(Cell::Type color);

        inline static void createConnection(Cell *c1, Cell *c2);

        void generateCellsAndConnections();

        inline bool visited(size_t row, size_t num) const {
            return getCell(row, num)->visited;
        }

        inline size_t getNeededEmptyCellsCount(CellType playerToCheck, size_t neededMoves) const;

        inline CellType currentPlayer() const;

        void clearVisited() const;

        void visitParents(const Cell *lastChild, const Cell *greatParent);

        bool canWin(size_t movesLeft, CellType color, data_structures::List<CellCoords> &emptyCellsCoords,
                    const WinVerifier &winVerifier);

        inline bool isGameWonBySomeone();

        inline bool enoughEmptyCells(CellType playerToCheck, size_t neededMoves) const;

        inline void fillEmptyCellsCoordsList(data_structures::List<CellCoords> &emptyCellsCoords,
                                             const CellDistanceEvaluator *evaluator = nullptr) const;

        size_t mSize;
        size_t redCellsCount;
        size_t blueCellsCount;
        Cell mRedBoarderLeft;
        Cell mRedBoarderRight;
        Cell mBlueBoarderLeft;
        Cell mBlueBoarderRight;
        data_structures::Vector<data_structures::Vector<Cell *> *> mBoard;

        friend class NeighboursGenerator;

        friend class RedNeighboursGenerator;

        friend class BlueNeighboursGenerator;

        friend class RedWinVerifier;

        friend class BlueWinVerifier;

        friend class BoardStateUtils;

        friend class RedDoneVerifier;

        friend class BlueDoneVerifier;
    };

} // board

#endif //HEX_BOARD_H
