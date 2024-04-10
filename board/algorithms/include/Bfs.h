//
// Created by stefan on 09/04/24.
//

#ifndef HEX_BFS_H
#define HEX_BFS_H

#include "Board.h"
#include "DoneVerifiers.h"
#include "List.h"
#include "Vector.h"
#include "NeighboursHelpers.h"


namespace board::algorithms {

    class Bfs {
    public:
        typedef NeighboursHelper::VisitedType VisitedType;
        typedef data_structures::Vector<data_structures::Vector<size_t>> DistancesType;

        Bfs(const Board &mBoard, const NeighboursHelper &mNeighboursHelper, const DoneVerifier &mDoneVerifier)
                : mBoard(mBoard), mNeighboursHelper(mNeighboursHelper), mDoneVerifier(mDoneVerifier) {}

        bool operator()(data_structures::List<CellCoords *> &nexts);

        static void fillDistancesForEmptyCells(const Board &board,
                                               DistancesType &distancesToLeftBorder,
                                               DistancesType &distancesToRightBorder,
                                               DistancesType &distancesToTopBorder,
                                               DistancesType &distancesToBottomBorder);

    private:
        static inline void fillDistancesToOneBorder(const Board &board, List<CellCoords *> &nexts,
                                                    EmptyNeighbourHelper &neighbourHelper);

        static inline void fillNextsAndUpdateDistancesInColumn(
                const Board &board, List<CellCoords *> &nexts, DistancesType &distances, size_t col, Cell::Type color);

        static inline void fillNextsAndUpdateDistancesInRow(
                const Board &board, List<CellCoords *> &nexts, DistancesType &distances, size_t row, Cell::Type color);

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

} // board::algorithms

#endif //HEX_BFS_H
