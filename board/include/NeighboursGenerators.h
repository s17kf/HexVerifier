//
// Created by stefan on 08/04/24.
//

#ifndef HEX_NEIGHBOURSGENERATORS_H
#define HEX_NEIGHBOURSGENERATORS_H

#include "List.h"
#include "CellCoords.h"
#include "NeighboursGenerators.h"

namespace board {
    class Board;
    class BoardStateUtils;

    class NeighboursGenerator {
    public:
        typedef Cell::Type CellType;

        explicit NeighboursGenerator(Board &mBoard) : mBoard(mBoard) {}

        virtual ~NeighboursGenerator() = default;

        virtual void fill(data_structures::List<CellCoords> &neighbours,
                          CellCoords cellCoords, bool emptyAllowed) const = 0;

        virtual void fillFirstLine(data_structures::List<CellCoords> &neighbours) const = 0;

        virtual void fillLastLine(data_structures::List<CellCoords> &neighbours) const = 0;

        virtual void fillStartLeafs(
                data_structures::List<CellCoords> &neighbours, const BoardStateUtils &boardStateUtils) const = 0;

        data_structures::List<CellCoords> get(
                const CellCoords &cellCoords, bool emptyAllowed) const;

    protected:
        void addNeighbourBelow(size_t row, size_t num, Cell *cell, CellType color,
                               data_structures::List<CellCoords> &neighbours, bool emptyAllowed) const;

        void addNeighbourAbove(size_t row, size_t num, Cell *cell, CellType color,
                               data_structures::List<CellCoords> &neighbours, bool emptyAllowed) const;

        void addNeighboursOnLeft(
                size_t row, size_t num, Cell *cell, CellType color,
                data_structures::List<CellCoords> &neighbours, bool emptyAllowed, bool topFirst) const;

        void addNeighboursOnRight(
                size_t row, size_t num, Cell *cell, CellType color,
                data_structures::List<CellCoords> &neighbours, bool emptyAllowed, bool topFirst) const;

        inline void addToListIfNotVisited(
                size_t row, size_t num, data_structures::List<CellCoords> &list, Cell *parent,
                CellCoords::Direction direction, CellType color, bool emptyAllowed) const;


        Board &mBoard;
    };

    class RedNeighboursGenerator : public NeighboursGenerator {
    public:
        explicit RedNeighboursGenerator(Board &mBoard) : NeighboursGenerator(mBoard) {}

        void fill(data_structures::List<CellCoords> &neighbours, CellCoords cellCoords,
                  bool emptyAllowed) const override;

        void fillFirstLine(data_structures::List<CellCoords> &neighbours) const override;

        void fillLastLine(data_structures::List<CellCoords> &neighbours) const override;

        void fillStartLeafs(
                data_structures::List<CellCoords> &neighbours, const BoardStateUtils &boardStateUtils) const override;
    };

    class BlueNeighboursGenerator : public NeighboursGenerator {
    public:
        explicit BlueNeighboursGenerator(Board &mBoard) : NeighboursGenerator(mBoard) {}

        void fill(data_structures::List<CellCoords> &neighbours, CellCoords cellCoords,
                  bool emptyAllowed) const override;

        void fillFirstLine(data_structures::List<CellCoords> &neighbours) const override;

        void fillLastLine(data_structures::List<CellCoords> &neighbours) const override;

        void fillStartLeafs(
                data_structures::List<CellCoords> &neighbours, const BoardStateUtils &boardStateUtils) const override;
    };

} // board

#endif //HEX_NEIGHBOURSGENERATORS_H
