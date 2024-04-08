//
// Created by stefan on 08/04/24.
//

#ifndef HEX_BOARDSTATEUTILS_H
#define HEX_BOARDSTATEUTILS_H

#include "NeighboursGenerators.h"
#include "Board.h"
#include "List.h"
#include "CellCoords.h"

namespace board {

    class BoardStateUtils {
    public:
        BoardStateUtils(Board &board);

        const RedNeighboursGenerator &getRedNeighbourGenerator() const {
            return mRedNeighbourGenerator;
        }

        const BlueNeighboursGenerator &getBlueNeighbourGenerator() const {
            return mBlueNeighbourGenerator;
        }

        const data_structures::List<CellCoords> &getRedStartLeafs() const {
            return mRedStartLeafs;
        }

        const data_structures::List<CellCoords> &getRedEndLeafs() const {
            return mRedEndLeafs;
        }

        const data_structures::List<CellCoords> &getBlueStartLeafs() const {
            return mBlueStartLeafs;
        }

        const data_structures::List<CellCoords> &getBlueEndLeafs() const {
            return mBlueEndLeafs;
        }

    private:
        void setLeafs(data_structures::List<CellCoords> &startLeafs, data_structures::List<CellCoords> &endLeafs,
                      const NeighboursGenerator &neighboursGenerator);

        void setOneSideLeafs(data_structures::List<CellCoords> &leafs,
                             const NeighboursGenerator &neighboursGenerator, bool startLeafs);

        const RedNeighboursGenerator mRedNeighbourGenerator;
        const BlueNeighboursGenerator mBlueNeighbourGenerator;
        const Board &mBoard;
        data_structures::List<CellCoords> mRedStartLeafs;
        data_structures::List<CellCoords> mRedEndLeafs;
        data_structures::List<CellCoords> mBlueStartLeafs;
        data_structures::List<CellCoords> mBlueEndLeafs;
    };

} // board

#endif //HEX_BOARDSTATEUTILS_H
