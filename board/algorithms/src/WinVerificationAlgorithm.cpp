//
// Created by stefan on 11/04/24.
//

#include "WinVerificationAlgorithm.h"
#include "Board.h"

void board::algorithms::WinVerificationAlgorithm::initVisited(
        data_structures::Vector<data_structures::Vector<bool>> &visited, const data_structures::List<CellCoords> &path,
        const Board &board) {
    for (auto &row: visited) {
        row.init(board.size());
    }
    for (const auto &coords: path) {
        visited[coords.row][coords.num] = true;
    }
}

