//
// Created by stefan on 10/04/24.
//

#ifndef HEX_WINVERIFICATIONALGORITHM_H
#define HEX_WINVERIFICATIONALGORITHM_H

#include "List.h"
#include "CellCoords.h"

namespace board::algorithms {

    class WinVerificationAlgorithm {
    public:
        virtual bool operator()(data_structures::List<CellCoords *> &startCoordsList,
                                data_structures::List<CellCoords> &path) const = 0;
    };

} // board::algorithms

#endif //HEX_WINVERIFICATIONALGORITHM_H
