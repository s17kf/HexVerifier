//
// Created by stefan on 10/04/24.
//

#ifndef HEX_WINVERIFICATIONALGORITHM_H
#define HEX_WINVERIFICATIONALGORITHM_H

#include "List.h"
#include "Vector.h"
#include "CellCoords.h"

namespace board {
    class Board;
    namespace algorithms {

        class WinVerificationAlgorithm {
        public:
            typedef data_structures::Vector<data_structures::Vector<bool>> VisitedType;

            virtual bool operator()(data_structures::List<CellCoords *> &startCoordsList,
                                    data_structures::List<CellCoords> &path) const = 0;

        protected:
            static void initVisited(VisitedType &visited, const data_structures::List<CellCoords> &path,
                                    const Board &board);
        };


    } // algorithms
} //board

#endif //HEX_WINVERIFICATIONALGORITHM_H
