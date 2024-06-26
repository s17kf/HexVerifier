//
// Created by stefan on 08/04/24.
//

#ifndef HEX_NEIGHBOURSHELPERS_H
#define HEX_NEIGHBOURSHELPERS_H

#include "List.h"
#include "Vector.h"
#include "CellCoords.h"
#include "Color.h"


namespace board {
    class Board;

    class NeighboursHelper {
    public:
        typedef data_structures::Vector<data_structures::Vector<bool>> VisitedType;

        explicit NeighboursHelper(const Board &mBoard) : mBoard(mBoard) {}

        NeighboursHelper(const NeighboursHelper &) = delete;

        virtual ~NeighboursHelper() = default;

        virtual void fill(data_structures::List<CellCoords *> &neighbours, const CellCoords *cellCoords,
                          const VisitedType *visited, bool emptyAllowed) const = 0;

    protected:
        inline void addNeighbourBelow(
                size_t row, size_t num, Color color, data_structures::List<CellCoords *> &neighbours,
                const VisitedType *visited, bool emptyAllowed) const;

        inline void addNeighbourAbove(
                size_t row, size_t num, Color color, data_structures::List<CellCoords *> &neighbours,
                const VisitedType *visited, bool emptyAllowed) const;

        inline void addNeighbourAboveLeft(
                size_t row, size_t num, Color color, data_structures::List<CellCoords *> &neighbours,
                const VisitedType *visited, bool emptyAllowed) const;

        inline void addNeighbourBelowRight(
                size_t row, size_t num, Color color, data_structures::List<CellCoords *> &neighbours,
                const VisitedType *visited, bool emptyAllowed) const;

        inline void addNeighbourLeft(
                size_t row, size_t num, Color color, data_structures::List<CellCoords *> &neighbours,
                const VisitedType *visited, bool emptyAllowed) const;

        inline void addNeighbourRight(
                size_t row, size_t num, Color color, data_structures::List<CellCoords *> &neighbours,
                const VisitedType *visited, bool emptyAllowed) const;

        virtual void addToListIfNotVisited(
                size_t row, size_t num, CellCoords::Direction direction, size_t parentRow, size_t parentNum,
                data_structures::List<CellCoords *> &list, const VisitedType *visited, Color color,
                bool emptyAllowed) const;


        const Board &mBoard;
    };

    class RedNeighboursHelper : public NeighboursHelper {
    public:
        explicit RedNeighboursHelper(const Board &mBoard) : NeighboursHelper(mBoard) {}

        void fill(data_structures::List<CellCoords *> &neighbours, const CellCoords *cellCoords,
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

        void
        fill(data_structures::List<CellCoords *> &neighbours, const CellCoords *cellCoords, const VisitedType *visited,
             bool emptyAllowed) const override;
    };

    class BlueStraightNeighbourHelper : public NeighboursHelper {
    public:
        explicit BlueStraightNeighbourHelper(const Board &mBoard) : NeighboursHelper(mBoard) {}

        void fill(
                data_structures::List<CellCoords *> &neighbours, const CellCoords *cellCoords,
                const VisitedType *visited, bool emptyAllowed) const override;
    };

    class DistanceUpdater {
    public:
        typedef data_structures::Vector<data_structures::Vector<size_t>> DistancesType;

        DistanceUpdater(const Board &board, DistancesType &distances, Color color) :
                mBoard(board), mDistances(distances), mColor(color) {}

        void updateDistance(size_t row, size_t num, size_t parentRow, size_t parentNum) const;

        [[nodiscard]] bool shouldVisit(size_t row, size_t num, size_t parentRow, size_t parentNum) const;

    private:
        const Board &mBoard;
        DistancesType &mDistances;
        Color mColor;
    };

    class EmptyNeighbourHelper : public NeighboursHelper {
    public:
        typedef data_structures::Vector<data_structures::Vector<size_t>> DistancesType;

        explicit EmptyNeighbourHelper(const Board &board, Color color, DistancesType &distances) :
                NeighboursHelper(board),
                mColor(color),
                mDistanceUpdater(board, distances, color) {}

        void fill(
                data_structures::List<CellCoords *> &neighbours, const CellCoords *cellCoords,
                const VisitedType *visited, bool emptyAllowed) const override;


    protected:
        void addToListIfNotVisited(
                size_t row, size_t num, CellCoords::Direction direction, size_t parentRow, size_t parentNum,
                data_structures::List<CellCoords *> &list, const VisitedType *visited, Color color,
                bool emptyAllowed) const override;


    private:
        Color mColor;
        DistanceUpdater mDistanceUpdater;
    };

} // board

#endif //HEX_NEIGHBOURSHELPERS_H
