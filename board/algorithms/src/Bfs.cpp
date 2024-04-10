//
// Created by stefan on 09/04/24.
//

#include "Bfs.h"


namespace board::algorithms {

    bool Bfs::operator()(List<CellCoords *> &nexts) {
        VisitedType visited(mBoard.size());
        for (auto &row: visited) {
            row.init(mBoard.size());
        }
        bool result = false;
        while (!nexts.empty()) {
            auto *next = nexts.popFront();
            if(visited[next->row][next->num]){
                delete next;
                continue;
            }
            visited[next->row][next->num] = true;
            if (mDoneVerifier(next)){
                result = true;
                delete next;
                break;
            }
            mNeighboursHelper.fill(nexts, next, &visited, false);
            delete next;
        }
        while(!nexts.empty()){
            delete nexts.popFront();
        }
        return result;
    }

    void Bfs::fillDistancesForEmptyCells(const Board &board,
                                         DistancesType &distancesToLeftBorder,
                                         DistancesType &distancesToRightBorder,
                                         DistancesType &distancesToTopBorder,
                                         DistancesType &distancesToBottomBorder) {
        initDistances(distancesToLeftBorder);
        initDistances(distancesToRightBorder);
        initDistances(distancesToTopBorder);
        initDistances(distancesToBottomBorder);
        {
            List<CellCoords *> nexts;
            fillNextsAndUpdateDistancesInColumn(board, nexts, distancesToLeftBorder, 0, Cell::Type::red);
            EmptyNeighbourHelper neighbourHelper(board, Cell::Type::red, distancesToLeftBorder);
            fillDistancesToOneBorder(board, nexts, neighbourHelper);
        }
        {
            List<CellCoords *> nexts;
            fillNextsAndUpdateDistancesInColumn(board, nexts, distancesToRightBorder, board.size() - 1,
                                                Cell::Type::red);
            EmptyNeighbourHelper neighbourHelper(board, Cell::Type::red, distancesToRightBorder);
            fillDistancesToOneBorder(board, nexts, neighbourHelper);
        }
        {
            List<CellCoords *> nexts;
            fillNextsAndUpdateDistancesInRow(board, nexts, distancesToTopBorder, 0, Cell::Type::blue);
            EmptyNeighbourHelper neighbourHelper(board, Cell::Type::blue, distancesToTopBorder);
            fillDistancesToOneBorder(board, nexts, neighbourHelper);
        }
        {
            List<CellCoords *> nexts;
            fillNextsAndUpdateDistancesInRow(board, nexts, distancesToBottomBorder, board.size() - 1,
                                             Cell::Type::blue);
            EmptyNeighbourHelper neighbourHelper(board, Cell::Type::blue, distancesToBottomBorder);
            fillDistancesToOneBorder(board, nexts, neighbourHelper);
        }
    }

    void Bfs::fillDistancesToOneBorder(const Board &board, List<CellCoords *> &nexts,
                                       EmptyNeighbourHelper &neighbourHelper) {
        VisitedType visited(board.size());
        for (auto &row: visited) {
            row.init(board.size());
        }
        while (!nexts.empty()) {
            auto *coords = nexts.popFront();
            neighbourHelper.fill(nexts, coords, nullptr);
            delete coords;
        }
    }

    void Bfs::fillNextsAndUpdateDistancesInColumn(const Board &board, List<CellCoords *> &nexts,
                                                  DistancesType &distances, size_t col, Cell::Type color) {
        for (size_t row = 0u; row < board.size(); ++row) {
            if (board.getType(row, col) == color) {
                distances[row][col] = 0;
                nexts.pushBack(new CellCoords{row, col});
                continue;
            }
            if (board.getType(row, col) == Cell::Type::empty) {
                distances[row][col] = 1;
                nexts.pushBack(new CellCoords{row, col});
            }
        }
    }

    void Bfs::fillNextsAndUpdateDistancesInRow(const Board &board, List<CellCoords *> &nexts,
                                               Bfs::DistancesType &distances, size_t row, Cell::Type color) {
        for (size_t num = 0u; num < board.size(); ++num) {
            if (board.getType(row, num) == color) {
                distances[row][num] = 0;
                nexts.pushBack(new CellCoords{row, num});
                continue;
            }
            if (board.getType(row, num) == Cell::Type::empty) {
                distances[row][num] = 1;
                nexts.pushBack(new CellCoords{row, num});
            }
        }
    }

} // board::algorithms
