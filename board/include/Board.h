//
// Created by stefan on 01/04/24.
//

#ifndef HEX_BOARD_H
#define HEX_BOARD_H

#include "Color.h"
#include "Vector.h"
#include "DoneVerifiers.h"
#include "NeighboursHelpers.h"
#include "DistancesKeeper.h"
#include "Bfs.h"


namespace board {
    namespace algorithms {
        class MinMax;
    }

    class Board {
    public:
        typedef data_structures::Vector<data_structures::Vector<size_t>> DistancesType;

        virtual ~Board() = default;

        explicit Board(size_t size);

        Board(const Board &) = delete;

        [[nodiscard]] inline size_t size() const {
            return mSize;
        }

        [[nodiscard]] size_t getColorCount(Color color) const;

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

        [[nodiscard]] inline Color getColor(size_t row, size_t num) const {
            return mBoard[row][num];
        }

        void setColor(size_t row, size_t num, Color color);

    private:
        inline void incColorCount(Color color);

        inline void decColorCount(Color color);

        [[nodiscard]] inline bool isGameWonByRed(const DistancesType &distancesToRightBorder) const;

        [[nodiscard]] inline bool isGameWonByBlue(const DistancesType &distancesTpBottomBorder) const;

        [[nodiscard]] inline bool isGameWonByRed(
                const algorithms::WinVerificationAlgorithm &algorithm, data_structures::List<CellCoords> &path) const;

        [[nodiscard]] inline bool isGameWonByBlue(
                const algorithms::WinVerificationAlgorithm &algorithm, data_structures::List<CellCoords> &path) const;

        [[nodiscard]] inline bool canWinInNMovesWithNaive(
                size_t n, bool movesFirst, const DistancesType &distancesToFirstBorder,
                const DistancesType &distancesToSecondBorder);

        inline bool canWinInNMovesWithPerfect(size_t n, bool movesFirst, Color playerColor,
                                              Color opponentColor, algorithms::MinMax &minMax,
                                              const DistancesType &playerDistances1,
                                              const DistancesType &playerDistances2,
                                              const DistancesType &opponentDistances1,
                                              const DistancesType &opponentDistances2);

        inline void fillEmptyCells(data_structures::List<CellCoords *> &cellList);

        inline void fillEmptyCellsForPlayer(data_structures::List<CellCoords> &cellList,
                                            const DistancesType &playerDistances1,
                                            const DistancesType &playerDistances2,
                                            size_t maxDistance);

        inline void fillEmptyCellsForOpponent(data_structures::List<CellCoords> &cellList,
                                              const DistancesType &playerDistances1,
                                              const DistancesType &playerDistances2,
                                              const DistancesType &opponentDistances1,
                                              const DistancesType &opponentDistances2,
                                              size_t playerMaxDistance,
                                              size_t opponentMaxDistance);

        static inline bool canBeWinningCell(
                const DistancesType &firstBorderDistances, const DistancesType &secondBorderDistances,
                size_t maxDistance, size_t row, size_t num) {
            return firstBorderDistances[row][num] == 1 && secondBorderDistances[row][num] <= maxDistance ||
                   firstBorderDistances[row][num] <= maxDistance && secondBorderDistances[row][num] == 1;
        }

        size_t mSize;
        size_t mRedCellsCount;
        size_t mBlueCellsCount;
        data_structures::Vector<data_structures::Vector<Color>> mBoard;
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
