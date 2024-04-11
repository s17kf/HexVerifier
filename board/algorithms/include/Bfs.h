//
// Created by stefan on 09/04/24.
//

#ifndef HEX_BFS_H
#define HEX_BFS_H

#include "WinVerificationAlgorithm.h"
#include "DoneVerifiers.h"
#include "List.h"
#include "Vector.h"
#include "NeighboursHelpers.h"


namespace board {
    class Board;

    namespace algorithms {

        class Bfs : public WinVerificationAlgorithm {
        public:
            typedef data_structures::Vector<data_structures::Vector<size_t>> DistancesType;

            Bfs(const Board &mBoard, const NeighboursHelper &mNeighboursHelper, const DoneVerifier &mDoneVerifier)
                    : mBoard(mBoard), mNeighboursHelper(mNeighboursHelper), mDoneVerifier(mDoneVerifier) {}

            bool operator()(data_structures::List<CellCoords *> &nexts,
                            data_structures::List<CellCoords> &path) const override;

            static void fillDistancesForEmptyCells(const Board &board,
                                                   DistancesType &distancesToLeftBorder,
                                                   DistancesType &distancesToRightBorder,
                                                   DistancesType &distancesToTopBorder,
                                                   DistancesType &distancesToBottomBorder);

        private:
            static inline void fillDistancesToOneBorder(const Board &board, data_structures::List<CellCoords *> &nexts,
                                                        EmptyNeighbourHelper &neighbourHelper);

            static inline void fillNextsAndUpdateDistancesInColumn(
                    const Board &board, data_structures::List<CellCoords *> &nexts, DistancesType &distances,
                    size_t col,
                    Color color);

            static inline void fillNextsAndUpdateDistancesInRow(
                    const Board &board, data_structures::List<CellCoords *> &nexts, DistancesType &distances,
                    size_t row,
                    Color color);

            static inline void initDistances(DistancesType &distances) {
                for (auto &row: distances) {
                    row.init(distances.size());
                    std::fill_n(row.begin(), distances.size(), SIZE_MAX);
                }
            }

            const Board &mBoard;
            const NeighboursHelper &mNeighboursHelper;
            const DoneVerifier &mDoneVerifier;
        };

    } // algorithms
} // board

#endif //HEX_BFS_H
