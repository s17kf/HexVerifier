//
// Created by stefan on 09/04/24.
//

#include "Dfs.h"

using data_structures::List;
using data_structures::Vector;


namespace board::algorithms {

    bool Dfs::operator()(const List<CellCoords *> &startCoordsList, List<CellCoords> &path) const {
        Vector<Vector<bool>> visited(mBoard.size());
        for (auto &row: visited) {
            row.init(mBoard.size());
        }
        for (const auto &coords: path) {
            visited[coords.row][coords.num] = true;
        }
        bool result = false;
        for (const auto *coords: startCoordsList) {
            if(visited[coords->row][coords->num])
                continue;
            if (handleNode(visited, coords, path)) {
                result = true;
                path.pushBack(*coords);
                break;
            }
        }
        for (auto *coords: startCoordsList) {
            delete coords;
        }
        return result;
    }

    bool Dfs::handleNode(Vector<Vector<bool>> &visited, const CellCoords *coords, List<CellCoords> &path) const {
        visited[coords->row][coords->num] = true;
        if (doneVerifier(coords)) {
            return true;
        }
        List<CellCoords *> nextCoordsList;
        neighboursHelper.fill(nextCoordsList, coords, &visited, false);
        bool result = false;
        for (const auto *nextCoords: nextCoordsList) {
            if (handleNode(visited, nextCoords, path)) {
                result = true;
                path.pushBack(*nextCoords);
                break;
            }
        }
        for (auto *nextCoords: nextCoordsList) {
            delete nextCoords;
        }
        return result;
    }
} // algorithms
// board