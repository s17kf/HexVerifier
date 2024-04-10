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
#include "Bfs.h"


namespace board {
//    namespace algorithms {
//        class WinVerificationAlgorithm;
//    }

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

        [[nodiscard]] bool isGameWonByRed(const DistancesKeeper &distancesKeeper) const;

        [[nodiscard]] bool isGameWonByBlue(const DistancesKeeper &distancesKeeper) const;

        [[nodiscard]] bool isGameWonByRed(const algorithms::WinVerificationAlgorithm &algorithm) const;

        [[nodiscard]] bool isGameWonByBlue(const algorithms::WinVerificationAlgorithm &algorithm) const;

        [[nodiscard]] bool isBoardPossible() const;

        bool canRedWinInNMovesWithNaive(size_t n, const DistancesKeeper &distancesKeeper);

        bool canBlueWinInNMovesWithNaive(size_t n, const DistancesKeeper &distancesKeeper);

        bool canRedWinInNMovesWithPerfect(size_t n, const DistancesKeeper &distancesKeeper);

        bool canBlueWinInNMovesWithPerfect(size_t n, const DistancesKeeper &distancesKeeper);

        [[nodiscard]] inline Cell::Type getType(size_t row, size_t num) const {
            return mBoard[row][num].getType();
        }

        void setType(size_t row, size_t num, CellType type);

    private:
        inline void incColorCount(Cell::Type color);

        inline void decColorCount(Cell::Type color);

        [[nodiscard]] inline bool isGameWonByRed(const DistancesType &distancesToRightBorder) const;

        [[nodiscard]] inline bool isGameWonByBlue(const DistancesType &distancesTpBottomBorder) const;

        [[nodiscard]] inline bool isGameWonByRed(
                const algorithms::WinVerificationAlgorithm &algorithm, data_structures::List<CellCoords> &path) const;

        [[nodiscard]] inline bool isGameWonByBlue(
                const algorithms::WinVerificationAlgorithm &algorithm, data_structures::List<CellCoords> &path) const;

        [[nodiscard]] inline bool canWinInNMovesWithNaive(
                size_t n, bool movesFirst, const DistancesType &distancesToFirstBorder,
                const DistancesType &distancesToSecondBorder);

        inline void fillEmptyCells(data_structures::List<CellCoords *> &cellList);

        inline void fillEmptyCells(
                data_structures::List<CellCoords> &cellList, const DistancesType &playerDistances1,
                const DistancesType &playerDistances2, size_t maxDistance);

        inline void fillEmptyCells(data_structures::List<CellCoords> &cellList,
                                   const DistancesType &playerDistances1,
                                   const DistancesType &playerDistances2,
                                   const DistancesType &opponentDistances1,
                                   const DistancesType &opponentDistances2,
                                   size_t playerMaxDistance,
                                   size_t opponentMaxDistance);

        size_t mSize;
        size_t mRedCellsCount;
        size_t mBlueCellsCount;
        data_structures::Vector<data_structures::Vector<Cell>> mBoard;
        RedDoneVerifier mRedDoneVerifier;
        BlueDoneVerifier mBlueDoneVerifier;
        RedNeighboursHelper mRedNeighboursHelper;
        BlueNeighboursHelper mBlueNeighboursHelper;
        RedStraightNeighbourHelper mRedStraightNeighboursHelper;
        BlueStraightNeighbourHelper mBlueStraightNeighboursHelper;
        const algorithms::Bfs mBfsForRed;
        const algorithms::Bfs mBfsForBlue;
    };

} // board

#endif //HEX_BOARD_H
