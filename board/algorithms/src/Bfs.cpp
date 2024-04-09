//
// Created by stefan on 09/04/24.
//

#include "Bfs.h"
#include "NeighboursHelpers.h"


namespace board::algorithms {

    void Bfs::fillDistancesForEmptyCells(Bfs::DistancesType &distancesToLeftBorder,
                                         Bfs::DistancesType &distancesToRightBorder,
                                         Bfs::DistancesType &distancesToTopBorder,
                                         Bfs::DistancesType &distancesToBottomBorder) {

        initDistances(distancesToLeftBorder);
        initDistances(distancesToRightBorder);
        initDistances(distancesToTopBorder);
        initDistances(distancesToBottomBorder);
        {
            List<CellCoords *> nexts;
            fillNextsAndUpdateDistancesInColumn(nexts, distancesToLeftBorder, 0, Cell::Type::red);
            EmptyNeighbourHelper neighbourHelper(mBoard, Cell::Type::red, distancesToLeftBorder);
            fillDistancesToOneBorder(nexts, neighbourHelper);
        }
        {
            List<CellCoords *> nexts;
            fillNextsAndUpdateDistancesInColumn(nexts, distancesToRightBorder, mBoard.size() - 1, Cell::Type::red);
            EmptyNeighbourHelper neighbourHelper(mBoard, Cell::Type::red, distancesToRightBorder);
            fillDistancesToOneBorder(nexts, neighbourHelper);
        }
        {
            List<CellCoords *> nexts;
            fillNextsAndUpdateDistancesInRow(nexts, distancesToTopBorder, 0, Cell::Type::blue);
            EmptyNeighbourHelper neighbourHelper(mBoard, Cell::Type::blue, distancesToTopBorder);
            fillDistancesToOneBorder(nexts, neighbourHelper);
        }
        {
            List<CellCoords *> nexts;
            fillNextsAndUpdateDistancesInRow(nexts, distancesToBottomBorder, mBoard.size() - 1, Cell::Type::blue);
            EmptyNeighbourHelper neighbourHelper(mBoard, Cell::Type::blue, distancesToBottomBorder);
            fillDistancesToOneBorder(nexts, neighbourHelper);
        }
    }

    void Bfs::fillDistancesToOneBorder(List<CellCoords *> &nexts, EmptyNeighbourHelper &neighbourHelper) {
        VisitedType visited(mBoard.size());
        for (auto &row: visited) {
            row.init(mBoard.size());
        }
        while (!nexts.empty()) {
            auto *coords = nexts.popFront();
            neighbourHelper.fill(nexts, coords, nullptr);
            delete coords;
        }
    }

    void Bfs::fillNextsAndUpdateDistancesInColumn(List<CellCoords *> &nexts, DistancesType &distances, size_t col,
                                                  Cell::Type color) {
        for (size_t row = 0u; row < mBoard.size(); ++row) {
            if (mBoard.getType(row, col) == color) {
                distances[row][col] = 0;
                nexts.pushBack(new CellCoords{row, col});
                continue;
            }
            if (mBoard.getType(row, col) == Cell::Type::empty) {
                distances[row][col] = 1;
                nexts.pushBack(new CellCoords{row, col});
            }
        }
    }

    void Bfs::fillNextsAndUpdateDistancesInRow(List<CellCoords *> &nexts, Bfs::DistancesType &distances, size_t row,
                                               Cell::Type color) {
        for (size_t num = 0u; num < mBoard.size(); ++num) {
            if (mBoard.getType(row, num) == color) {
                distances[row][num] = 0;
                nexts.pushBack(new CellCoords{row, num});
                continue;
            }
            if (mBoard.getType(row, num) == Cell::Type::empty) {
                distances[row][num] = 1;
                nexts.pushBack(new CellCoords{row, num});
            }
        }
    }

} // board::algorithms
