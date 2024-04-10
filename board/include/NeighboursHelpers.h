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
        typedef Vector <Vector<bool>> VisitedType;

        explicit NeighboursHelper(const Board &mBoard) : mBoard(mBoard) {}

        NeighboursHelper(const NeighboursHelper&) = delete;

        virtual ~NeighboursHelper() = default;

        virtual void fill(List<CellCoords *> &neighbours, const CellCoords *cellCoords,
                          const VisitedType *visited, bool emptyAllowed) const = 0;

        List<CellCoords *> get(const CellCoords *cellCoords, const VisitedType *visited,
                               bool emptyAllowed) const;

    protected:
        void addNeighbourBelow(size_t row, size_t num, Cell::Type color, List<CellCoords *> &neighbours,
                               const VisitedType *visited, bool emptyAllowed) const;

        void addNeighbourAbove(size_t row, size_t num, Cell::Type color, List<CellCoords *> &neighbours,
                               const VisitedType *visited, bool emptyAllowed) const;

        void addNeighbourAboveLeft(size_t row, size_t num, Cell::Type color, List<CellCoords *> &neighbours,
                                   const VisitedType *visited, bool emptyAllowed) const;

        void addNeighbourBelowRight(size_t row, size_t num, Cell::Type color, List<CellCoords *> &neighbours,
                                    const VisitedType *visited, bool emptyAllowed) const;

        void addNeighbourLeft(size_t row, size_t num, Cell::Type color, List<CellCoords *> &neighbours,
                              const VisitedType *visited, bool emptyAllowed) const;

        void addNeighbourRight(size_t row, size_t num, Cell::Type color, List<CellCoords *> &neighbours,
                               const VisitedType *visited, bool emptyAllowed) const;

        virtual void addToListIfNotVisited(
                size_t row, size_t num, CellCoords::Direction direction, size_t parentRow, size_t parentNum,
                List<CellCoords *> &list, const VisitedType *visited, Cell::Type color, bool emptyAllowed) const;


        const Board &mBoard;
    };

    class RedNeighboursHelper : public NeighboursHelper {
    public:
        explicit RedNeighboursHelper(const Board &mBoard) : NeighboursHelper(mBoard) {}

        void fill(List<CellCoords *> &neighbours, const CellCoords *cellCoords,
                  const VisitedType *visited, bool emptyAllowed) const override;
    };

    class BlueNeighboursHelper : public NeighboursHelper {
    public:
        explicit BlueNeighboursHelper(const Board &mBoard) : NeighboursHelper(mBoard) {}

        void fill(data_structures::List<CellCoords *> &neighbours, const CellCoords *cellCoords,
                  const VisitedType *visited, bool emptyAllowed) const override;
    };

    class RedStraightNeighbourHelper : public NeighboursHelper {
    public:
        explicit RedStraightNeighbourHelper(const Board &mBoard) : NeighboursHelper(mBoard) {}

        void fill(List<CellCoords *> &neighbours, const CellCoords *cellCoords, const VisitedType *visited,
                  bool emptyAllowed) const override;
    };

    class BlueStraightNeighbourHelper : public NeighboursHelper {
    public:
        explicit BlueStraightNeighbourHelper(const Board &mBoard) : NeighboursHelper(mBoard) {}

        void fill(List<CellCoords *> &neighbours, const CellCoords *cellCoords, const VisitedType *visited,
                  bool emptyAllowed) const override;
    };

    class DistanceUpdater {
    public:
        typedef Vector <Vector<size_t>> DistancesType;

        DistanceUpdater(const Board &board, DistancesType &distances, Cell::Type color) :
                mBoard(board), mDistances(distances), mColor(color) {}

        void updateDistance(size_t row, size_t num, size_t parentRow, size_t parentNum) const;

        bool shouldVisit(size_t row, size_t num, size_t parentRow, size_t parentNum) const;

    private:
        const Board &mBoard;
        DistancesType &mDistances;
        Cell::Type mColor;
    };

    class EmptyNeighbourHelper : public NeighboursHelper {
    public:
        typedef Vector <Vector<size_t>> DistancesType;

        explicit EmptyNeighbourHelper(const Board &board, Cell::Type color, DistancesType &distances) :
                NeighboursHelper(board),
                mColor(color),
                mDistanceUpdater(board, distances, color) {}

        void fill(List<CellCoords *> &neighbours, const CellCoords *cellCoords, const VisitedType *visited,
                  bool emptyAllowed = true) const override;


    protected:
        void addToListIfNotVisited(size_t row, size_t num, CellCoords::Direction direction, size_t parentRow,
                                   size_t parentNum, List<CellCoords *> &list, const VisitedType *visited,
                                   Cell::Type color, bool emptyAllowed) const override;


    private:
        Cell::Type mColor;
        DistanceUpdater mDistanceUpdater;
    };

} // board

#endif //HEX_NEIGHBOURSHELPERS_H
