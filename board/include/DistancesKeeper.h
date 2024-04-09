//
// Created by stefan on 10/04/24.
//

#ifndef HEX_DISTANCESKEEPER_H
#define HEX_DISTANCESKEEPER_H

#include "Vector.h"

namespace board {
    class Board;

    class DistancesKeeper {
    public:
        typedef data_structures::Vector<data_structures::Vector<size_t>> DistancesType;

        explicit DistancesKeeper(const Board &board);


        [[nodiscard]] const DistancesType &getDistancesToLeftBorder() const {
            return distancesToLeftBorder;
        }

        [[nodiscard]] const DistancesType &getDistancesToRightBorder() const {
            return distancesToRightBorder;
        }

        [[nodiscard]] const DistancesType &getDistancesToTopBorder() const {
            return distancesToTopBorder;
        }

        [[nodiscard]] const DistancesType &getDistancesToBottomBorder() const {
            return distancesToBottomBorder;
        }

    private:
        DistancesType distancesToLeftBorder;
        DistancesType distancesToRightBorder;
        DistancesType distancesToTopBorder;
        DistancesType distancesToBottomBorder;
    };

} // board

#endif //HEX_DISTANCESKEEPER_H
