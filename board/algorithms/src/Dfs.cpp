//
// Created by stefan on 09/04/24.
//

#include "Dfs.h"

using data_structures::List;
using data_structures::Vector;


namespace board::algorithms {
    bool Dfs::operator()(const List<CellCoords *> &startCoordsList) const {
        Vector<Vector<bool>> visited(mBoard.size());
        for (auto &row: visited) {
            row.init(mBoard.size());
        }
        bool result = false;
        for (const auto *coords: startCoordsList) {
            if (handleNode(visited, coords)) {
                result = true;
                break;
            }
        }
        for (auto *coords: startCoordsList) {
            delete coords;
        }
        return result;
    }

    bool Dfs::handleNode(Vector <Vector<bool>> &visited, const CellCoords *coords) const {
        visited[coords->row][coords->num] = true;
        if (doneVerifier(coords)) {
            return true;
        }
        List<CellCoords *> nextCoordsList;
        neighboursHelper.fill(nextCoordsList, coords, visited, false);
        bool result = false;
        for (const auto *nextCoords: nextCoordsList) {
            if (handleNode(visited, nextCoords)) {
                result = true;
                break;
            }
        }
        for (auto *nextCoordsList1: nextCoordsList) {
            delete nextCoordsList1;
        }
        return result;
    }
} // algorithms
// board