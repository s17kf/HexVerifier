//
// Created by stefan on 09/04/24.
//

#ifndef HEX_DFS_H
#define HEX_DFS_H

#include "WinVerificationAlgorithm.h"
#include "Board.h"
#include "NeighboursHelpers.h"
#include "DoneVerifiers.h"
#include "List.h"
#include "Vector.h"


namespace board::algorithms {

    class Dfs : public WinVerificationAlgorithm {
    public:
        Dfs(const Board &mBoard, const NeighboursHelper &neighboursHelper, const DoneVerifier &doneVerifier)
                : mBoard(mBoard), neighboursHelper(neighboursHelper), doneVerifier(doneVerifier) {}

        bool operator()(data_structures::List<CellCoords *> &startCoordsList,
                        data_structures::List<CellCoords> &path) const override;

    private:
        bool handleNode(VisitedType &visited,
                        const CellCoords *coords, data_structures::List<CellCoords> &path) const;

        const Board &mBoard;
        const NeighboursHelper &neighboursHelper;
        const DoneVerifier &doneVerifier;
    };

} // algorithms
// board

#endif //HEX_DFS_H
