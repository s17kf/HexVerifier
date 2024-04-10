//
// Created by stefan on 09/04/24.
//

#ifndef HEX_DFS_H
#define HEX_DFS_H

#include "Board.h"
#include "NeighboursHelpers.h"
#include "DoneVerifiers.h"
#include "List.h"
#include "Vector.h"


namespace board::algorithms {

    class Dfs {
    public:
        typedef NeighboursHelper::VisitedType VisitedType;

        Dfs(const Board &mBoard, const NeighboursHelper &neighboursHelper, const DoneVerifier &doneVerifier)
                : mBoard(mBoard), neighboursHelper(neighboursHelper), doneVerifier(doneVerifier) {}

        bool operator()(const data_structures::List<CellCoords *> &startCoordsList, List<CellCoords> &path) const;

    private:
        bool handleNode(data_structures::Vector<data_structures::Vector<bool>> &visited,
                        const CellCoords *coords, List<CellCoords> &path) const;

        const Board &mBoard;
        const NeighboursHelper &neighboursHelper;
        const DoneVerifier &doneVerifier;
    };

} // algorithms
// board

#endif //HEX_DFS_H
