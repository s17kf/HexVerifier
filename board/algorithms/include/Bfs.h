//
// Created by stefan on 09/04/24.
//

#ifndef HEX_BFS_H
#define HEX_BFS_H

#include "Board.h"
#include "DoneVerifiers.h"
#include "List.h"
#include "Vector.h"


namespace board::algorithms {

    class Bfs {
    public:
        typedef NeighboursHelper::VisitedType VisitedType;
        typedef data_structures::Vector<data_structures::Vector<size_t>> DistancesType;

        Bfs(const Board &board) : mBoard(board) {}


        void fillDistancesForEmptyCells(DistancesType &distancesToLeftBorder,
                                        DistancesType &distancesToRightBorder,
                                        DistancesType &distancesToTopBorder,
                                        DistancesType &distancesToBottomBorder);

    private:
        inline void fillDistancesToOneBorder(List<CellCoords *> &nexts,
                                             EmptyNeighbourHelper &neighbourHelper);

        inline void fillNextsAndUpdateDistancesInColumn(List<CellCoords *> &nexts, DistancesType &distances, size_t col,
                                                        Cell::Type color);

        inline void fillNextsAndUpdateDistancesInRow(List<CellCoords *> &nexts, DistancesType &distances, size_t row,
                                                     Cell::Type color);

        const Board &mBoard;
    };

} // board::algorithms

#endif //HEX_BFS_H
