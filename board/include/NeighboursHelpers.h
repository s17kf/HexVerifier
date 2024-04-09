//
// Created by stefan on 08/04/24.
//

#ifndef HEX_NEIGHBOURSHELPERS_H
#define HEX_NEIGHBOURSHELPERS_H

#include "List.h"
#include "Vector.h"
#include "CellCoords.h"

using data_structures::List;
using data_structures::Vector;

namespace board {
    class Board;

    class NeighboursHelper {
    public:
        typedef Cell::Type CellType;

        explicit NeighboursHelper(const Board &mBoard) : mBoard(mBoard) {}

        virtual ~NeighboursHelper() = default;

        virtual void fill(List<CellCoords *> &neighbours, const CellCoords *cellCoords,
                          const Vector <Vector<bool>> &visited, bool emptyAllowed) const = 0;

        List<CellCoords *> get(const CellCoords *cellCoords, const Vector <Vector<bool>> &visited,
                               bool emptyAllowed) const;

    protected:
        void addNeighbourBelow(size_t row, size_t num, CellType color, List<CellCoords *> &neighbours,
                               const Vector <Vector<bool>> &visited, bool emptyAllowed) const;

        void addNeighbourAbove(size_t row, size_t num, CellType color, List<CellCoords *> &neighbours,
                               const Vector <Vector<bool>> &visited, bool emptyAllowed) const;

        void addNeighbourAboveLeft(size_t row, size_t num, CellType color, List<CellCoords *> &neighbours,
                                   const Vector <Vector<bool>> &visited, bool emptyAllowed) const;

        void addNeighbourBelowRight(size_t row, size_t num, CellType color, List<CellCoords *> &neighbours,
                                    const Vector <Vector<bool>> &visited, bool emptyAllowed) const;

        void addNeighbourLeft(size_t row, size_t num, CellType color, List<CellCoords *> &neighbours,
                              const Vector <Vector<bool>> &visited, bool emptyAllowed) const;

        void addNeighbourRight(size_t row, size_t num, CellType color, List<CellCoords *> &neighbours,
                               const Vector <Vector<bool>> &visited, bool emptyAllowed) const;

        inline void addToListIfNotVisited(
                size_t row, size_t num, CellCoords::Direction direction, List<CellCoords *> &list,
                const Vector <Vector<bool>> &visited, CellType color, bool emptyAllowed) const;


        const Board &mBoard;
    };

    class RedNeighboursHelper : public NeighboursHelper {
    public:
        explicit RedNeighboursHelper(const Board &mBoard) : NeighboursHelper(mBoard) {}

        void fill(List<CellCoords *> &neighbours, const CellCoords *cellCoords,
                  const Vector <Vector<bool>> &visited, bool emptyAllowed) const override;
    };

    class BlueNeighboursHelper : public NeighboursHelper {
    public:
        explicit BlueNeighboursHelper(const Board &mBoard) : NeighboursHelper(mBoard) {}

        void fill(data_structures::List<CellCoords *> &neighbours, const CellCoords *cellCoords,
                  const Vector <Vector<bool>> &visited, bool emptyAllowed) const override;
    };

} // board

#endif //HEX_NEIGHBOURSHELPERS_H
