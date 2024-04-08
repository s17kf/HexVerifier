//
// Created by stefan on 08/04/24.
//

#include "BoardStateUtils.h"

using data_structures::List;

namespace board {
    BoardStateUtils::BoardStateUtils(Board &board) : mBoard(board),
                                                     mRedNeighbourGenerator(board),
                                                     mBlueNeighbourGenerator(board) {
        setLeafs(mRedStartLeafs, mRedEndLeafs, mRedNeighbourGenerator);
        setLeafs(mBlueStartLeafs, mBlueEndLeafs, mBlueNeighbourGenerator);
    }

    void BoardStateUtils::setLeafs(List <CellCoords> &startLeafs, List <CellCoords> &endLeafs,
                                   const NeighboursGenerator &neighboursGenerator) {
        setOneSideLeafs(startLeafs, neighboursGenerator, true);
        setOneSideLeafs(endLeafs, neighboursGenerator, false);
    }

    void BoardStateUtils::setOneSideLeafs(List <CellCoords> &leafs,
                                          const NeighboursGenerator &neighboursGenerator, bool startLeafs) {
        List<CellCoords> nexts;
        if (startLeafs)
            neighboursGenerator.fillFirstLine(nexts);
        else
            neighboursGenerator.fillLastLine(nexts);

        while (!nexts.empty()) {
            auto coords = nexts.popFront();
            auto *cell = mBoard.getCell(coords);
            if (cell->visited)
                continue;
            cell->visited = true;
            for (auto nextCoords: neighboursGenerator.get(coords, true)) {
                if (mBoard.getCell(nextCoords)->getType() == Cell::Type::empty) {
                    if (startLeafs)
                        cell->isStartLeaf = true;
                    else
                        cell->isEndLeaf = true;
                    continue;
                }
                nexts.pushBack(nextCoords);
            }
            if ((cell->isStartLeaf || cell->isEndLeaf)) {
                leafs.pushBack(coords);
            }
        }
        mBoard.clearVisited();
    }

} // board