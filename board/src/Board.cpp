//
// Created by stefan on 01/04/24.
//

#include "Board.h"

#include "DoneVerifiers.h"
#include "NeighboursHelpers.h"
#include "Dfs.h"


using data_structures::Vector;
using data_structures::List;

namespace board {
    using algorithms::Dfs;

    Board::Board(size_t size) : mSize(size),
                                mRedCellsCount(0),
                                mBlueCellsCount(0),
                                mBoard(size),
                                mRedDoneVerifier(*this),
                                mBlueDoneVerifier(*this),
                                mRedNeighboursHelper(*this),
                                mBlueNeighboursHelper(*this) {
        for (auto &row: mBoard) {
            row.init(size);
        }
    }


    size_t Board::getColorCount(Cell::Type color) const {
        switch (color) {
            case Cell::Type::red:
                return mRedCellsCount;
            case Cell::Type::blue:
                return mBlueCellsCount;
            case Cell::Type::empty:
                return mSize * mSize - mRedCellsCount - mBlueCellsCount;
        }
        throw std::invalid_argument("Get color count - invalid color!");
    }


    bool Board::isBoardCorrect() const {
        auto redPawns = getColorCount(Cell::Type::red);
        auto bluePawns = getColorCount(Cell::Type::blue);
        if (redPawns < bluePawns || redPawns > bluePawns + 1) {
            return false;
        }
        return true;
    }

    bool Board::isGameWon(Cell::Type color) const {
        switch (color) {
            case Cell::Type::red: {
                Dfs dfs(*this, mRedNeighboursHelper, mRedDoneVerifier);
                List<CellCoords *> startCoordsList;
                for (size_t row = 0u; row < size(); ++row) {
                    if (getType(row, 0) == Cell::Type::red)
                        startCoordsList.pushBack(new CellCoords{row, 0, CellCoords::Direction::right});
                }
                return dfs(startCoordsList);
            }
            case Cell::Type::blue: {
                Dfs dfs(*this, mBlueNeighboursHelper, mBlueDoneVerifier);
                List<CellCoords *> startCoordsList;
                for (size_t num = 0u; num < size(); ++num) {
                    if (getType(0, num) == Cell::Type::blue)
                        startCoordsList.pushBack(new CellCoords{0, num, CellCoords::Direction::left});
                }
                return dfs(startCoordsList);
            }
            default:
                throw std::invalid_argument("Only blue and red colors are allowed for win verification");
        }
    }

    void Board::setType(size_t row, size_t num, CellType type) {
        if (getType(row, num) != Cell::Type::empty)
            decColorCount(getType(row, num));
        if (type != Cell::Type::empty)
            incColorCount(type);

        mBoard[row][num].setType(type);
    }

    void Board::incColorCount(Cell::Type color) {
        switch (color) {
            case Cell::Type::red:
                ++mRedCellsCount;
                return;
            case Cell::Type::blue:
                ++mBlueCellsCount;
                return;
            default:
                throw std::invalid_argument("Try to increment count of not allowed type of cell!");
        }

    }

    void Board::decColorCount(Cell::Type color) {
        switch (color) {
            case Cell::Type::red:
                --mRedCellsCount;
                return;
            case Cell::Type::blue:
                --mBlueCellsCount;
                return;
            default:
                throw std::invalid_argument("Try to decrement count of not allowed type of cell!");
        }
    }


} // board