//
// Created by stefan on 01/04/24.
//

#include "Board.h"

using data_structures::Vector;
using data_structures::List;

namespace board {

    Board::Board(size_t size) : mSize(size),
                                redCellsCount(0),
                                blueCellsCount(0),
                                mRedBoarderLeft(Cell::Type::redBorderLeft),
                                mRedBoarderRight(Cell::Type::redBorderRight),
                                mBlueBoarderLeft(Cell::Type::blueBorderLeft),
                                mBlueBoarderRight(Cell::Type::blueBorderRight),
                                mBoard(2 * size - 1) {
        generateCellsAndConnections();
    }

    Board::Board(const Board &other) : mSize(other.mSize),
                                       redCellsCount(0),
                                       blueCellsCount(0),
                                       mRedBoarderLeft(Cell::Type::redBorderLeft),
                                       mRedBoarderRight(Cell::Type::redBorderRight),
                                       mBlueBoarderLeft(Cell::Type::blueBorderLeft),
                                       mBlueBoarderRight(Cell::Type::blueBorderRight),
                                       mBoard(2 * mSize - 1) {
        generateCellsAndConnections();
        for (size_t rowNum = 0u; rowNum < mBoard.size(); ++rowNum) {
            auto row = mBoard[rowNum];
            for (size_t cellNum = 0u; cellNum < row->size(); ++cellNum) {
                if (other.getCell(rowNum, cellNum)->getType() != Cell::Type::empty) {
                    setType(rowNum, cellNum, other.getCell(rowNum, cellNum)->getType());
                }
            }
        }
    }

    Board::~Board() {
        for (auto *row: mBoard) {
            for (auto *cell: *row) {
                delete cell;
            }
            delete row;
        }
    }

    const Cell &Board::getBoarder(Cell::Type type) const {
        switch (type) {
            case Cell::Type::redBorderRight:
                return mRedBoarderRight;
            case Cell::Type::redBorderLeft:
                return mRedBoarderLeft;
            case Cell::Type::blueBorderRight:
                return mBlueBoarderRight;
            case Cell::Type::blueBorderLeft:
                return mBlueBoarderLeft;
            default:
                throw std::invalid_argument("Not boarder type!");
        }
    }

    void Board::createConnection(Cell *c1, Cell *c2) {
        c1->addNeighbour(c2);
        c2->addNeighbour(c1);
    }

    void Board::setType(size_t row, size_t cellNum, Board::CellType type) {
        if (type > Cell::Type::blue) {
            throw std::invalid_argument("Try to set not allowed type of cell (boarder type used)!");
        }
        if (getCell(row, cellNum)->getType() != Cell::Type::empty)
            decColorCount(getCell(row, cellNum)->getType());
        if (type != Cell::Type::empty)
            incColorCount(type);

        mBoard[row]->at(cellNum)->setType(type);
    }

    size_t Board::getColorCount(Cell::Type color) const {
        switch (color) {
            case Cell::Type::red:
                return redCellsCount;
            case Cell::Type::blue:
                return blueCellsCount;
            default:
                throw std::invalid_argument("Try to get count of not allowed type of cell!");
        }
    }

    void Board::incColorCount(Cell::Type color) {
        switch (color) {
            case Cell::Type::red:
                ++redCellsCount;
                return;
            case Cell::Type::blue:
                ++blueCellsCount;
                return;
            default:
                throw std::invalid_argument("Try to increment count of not allowed type of cell!");
        }
    }

    void Board::decColorCount(Cell::Type color) {
        switch (color) {
            case Cell::Type::red:
                --redCellsCount;
                return;
            case Cell::Type::blue:
                --blueCellsCount;
                return;
            default:
                throw std::invalid_argument("Try to decrement count of not allowed type of cell!");
        }

    }

    bool Board::isBoardCorrect() const {
        auto redPawns = getColorCount(Cell::Type::red);
        auto bluePawns = getColorCount(Cell::Type::blue);
        if (redPawns < bluePawns || redPawns > bluePawns + 1) {
            return false;
        }
        return true;
    }

    bool Board::isBoardPossible() const {
        if (!isBoardCorrect())
            return false;
        auto redPawns = getColorCount(Cell::Type::red);
        auto bluePawns = getColorCount(Cell::Type::blue);
        if (bluePawns == redPawns && isRedWinOnce())
            return false;
        if (bluePawns < redPawns && isBlueWinOnce())
            return false;
        return true;
    }

    bool Board::isRedWin() const {
        Board copy(*this);
        List<CellCoords> nexts;
        for (size_t row = 0u; row < copy.mSize; ++row) {
            if (mBoard[row]->at(0)->getType() == Cell::Type::red)
                nexts.pushBack({row, 0, &copy.mRedBoarderRight});
        }
        return copy.bfs(nexts, Cell::Type::red, copy.mRedBoarderRight, Board::doneForRed, Board::getNeighboursForRed);
    }

    bool Board::isRedWinOnce() const {
        Board copy(*this);
        {
            List<CellCoords> nexts;
            for (size_t row = 0u; row < copy.mSize; ++row) {
                if (mBoard[row]->at(0)->getType() == Cell::Type::red)
                    nexts.pushBack({row, 0, &copy.mRedBoarderRight});
            }
            if (!copy.bfs(nexts, Cell::Type::red, copy.mRedBoarderRight, Board::doneForRed, Board::getNeighboursForRed))
                return false;
        }
        copy.clearVisited();
        copy.visitParents(&copy.mRedBoarderRight, &copy.mRedBoarderLeft);
        {
            List<CellCoords> nexts;
            for (size_t row = 0u; row < copy.mSize; ++row) {
                if (!mBoard[row]->at(0)->visited && mBoard[row]->at(0)->getType() == Cell::Type::red)
                    nexts.pushBack({row, 0, &copy.mRedBoarderRight});
            }
            if (copy.bfs(nexts, Cell::Type::red, copy.mRedBoarderRight, Board::doneForRed, Board::getNeighboursForRed))
                return false;
        }
        return true;
    }

    bool Board::isBlueWin() const {
        Board copy(*this);
        List<CellCoords> nexts;
        for (size_t row = 0u; row < copy.mSize; ++row) {
            if (mBoard[row]->last()->getType() == Cell::Type::blue)
                nexts.pushBack({row, mBoard[row]->size() - 1, &copy.mBlueBoarderRight});
        }
        return copy.bfs(nexts, Cell::Type::blue, copy.mBlueBoarderLeft, Board::doneForBlue, Board::getNeighboursForBlue);
    }

    bool Board::isBlueWinOnce() const {
        Board copy(*this);
        {
            List<CellCoords> nexts;
            for (size_t row = 0u; row < copy.mSize; ++row) {
                if (mBoard[row]->last()->getType() == Cell::Type::blue)
                    nexts.pushBack({row, mBoard[row]->size() - 1, &copy.mBlueBoarderRight});
            }
            if (!copy.bfs(nexts, Cell::Type::blue, copy.mBlueBoarderLeft, Board::doneForBlue, Board::getNeighboursForBlue))
                return false;
        }
        copy.clearVisited();
        copy.visitParents(&copy.mBlueBoarderLeft, &copy.mBlueBoarderRight);
        {
            List<CellCoords> nexts;
            for (size_t row = 0u; row < copy.mSize; ++row) {
                if (!mBoard[row]->last()->visited && mBoard[row]->last()->getType() == Cell::Type::blue)
                    nexts.pushBack({row, mBoard[row]->size() - 1, &copy.mBlueBoarderRight});
            }
            if (copy.bfs(nexts, Cell::Type::blue, copy.mBlueBoarderLeft, Board::doneForBlue, Board::getNeighboursForBlue))
                return false;
        }
        return true;
    }

    bool Board::doneForBlue(const Board &board, size_t row, size_t num) {
        return row >= board.mSize - 1 && num == 0;
    }

    bool Board::doneForRed(const Board &board, size_t row, size_t num) {
        return row >= board.mSize - 1 && num == board.mBoard[row]->size() - 1;
    }

    List <Board::CellCoords> Board::getNeighboursForBlue(const Board &board, size_t row, size_t num) {
        List<CellCoords> neighbours;
        board.addNeighbourAbove(row, num, neighbours);
        board.addNeighbourOnLeft(row, num, neighbours);
        board.addNeighbourBelow(row, num, neighbours);
        board.addNeighbourOnRight(row, num, neighbours);
        return neighbours;
    }

    List <Board::CellCoords> Board::getNeighboursForRed(const Board &board, size_t row, size_t num) {
        List<CellCoords> neighbours;
        board.addNeighbourAbove(row, num, neighbours);
        board.addNeighbourOnRight(row, num, neighbours);
        board.addNeighbourBelow(row, num, neighbours);
        board.addNeighbourOnLeft(row, num, neighbours);
        return neighbours;
    }

    void Board::addNeighbourAbove(size_t row, size_t num, List <CellCoords> &neighbours) const {
        if (row > 1) {
            if (row == mSize) {
                neighbours.pushBack({row - 2, num});
            } else {
                if (row < mSize) {
                    if (num > 0 && num < mBoard[row]->size() - 1)
                        neighbours.pushBack({row - 2, num - 1});
                } else {
                    neighbours.pushBack({row - 2, num + 1});
                }
            }
        }
    }

    void Board::addNeighbourBelow(size_t row, size_t num, List <CellCoords> &neighbours) const {
        if (row < mBoard.size() - 2) {
            if (row == mSize - 2) {
                neighbours.pushBack({row + 2, num});
            } else {
                if (row < mSize - 1) {
                    neighbours.pushBack({row + 2, num + 1});
                } else {
                    if (num > 0 && num < mBoard[row]->size() - 1)
                        neighbours.pushBack({row + 2, num - 1});
                }
            }
        }
    }

    void Board::addNeighbourOnLeft(size_t row, size_t num, List <CellCoords> &neighbours) const {
        if (row == mSize - 1) {
            if (num > 0) {
                neighbours.pushBack({row - 1, num - 1});
                neighbours.pushBack({row + 1, num - 1});
            }
        } else {
            if (row < mSize) {
                neighbours.pushBack({row + 1, num});
                if (num > 0)
                    neighbours.pushBack({row - 1, num - 1});
            } else {
                neighbours.pushBack({row - 1, num});
                if (num > 0)
                    neighbours.pushBack({row + 1, num - 1});
            }
        }
    }

    void Board::addNeighbourOnRight(size_t row, size_t num, List <CellCoords> &neighbours) const {
        if (row == mSize - 1) {
            if (num < mSize - 1) {
                neighbours.pushBack({row - 1, num});
                neighbours.pushBack({row + 1, num});
            }
        } else {
            if (row < mSize) {
                neighbours.pushBack({row + 1, num + 1});
                if (row > 0 && num < mBoard[row]->size() - 1) {
                    neighbours.pushBack({row - 1, num});
                }
            } else {
                neighbours.pushBack({row - 1, num + 1});
                if (row < mBoard.size() - 1 && num < mBoard[row]->size() - 1) {
                    neighbours.pushBack({row + 1, num});
                }
            }
        }
    }


    bool Board::bfs(Cell *start, Cell *end, Board::CellType acceptedType) {
        data_structures::List<CellParent> nexts;
        for (auto *cell: start->getNeighbours()) {
            cell->parent = start;
            if (cell->getType() == acceptedType && !cell->visited) {
                nexts.pushBack({cell, start});
            }
        }
        while (!nexts.empty()) {
            auto cellParent = nexts.popFront();
            auto *cell = cellParent.cell;
            if (cell->visited)
                continue;
            cell->visited = true;
            cell->parent = cellParent.parent;
            for (auto *next: cell->getNeighbours()) {
                if (next == end) {
                    end->parent = cell;
                    return true;
                }
                if (!next->visited && next->getType() == acceptedType) {
                    nexts.pushBack({next, cell});
                }
            }
        }
        return false;
    }

    bool Board::bfs(data_structures::List<CellCoords> &nexts,
                    CellType color,
                    Cell &endBorder,
                    const std::function<bool(const Board &, size_t, size_t)> &done,
                    const std::function<List<CellCoords>(const Board &, size_t, size_t)> &getNeighbours) {
        while (!nexts.empty()) {
            auto cellCoords = nexts.popFront();
            auto *cell = getCell(cellCoords.row, cellCoords.num);
            if (cell->visited || cell->getType() != color)
                continue;
            cell->visited = true;
            cell->parent = cellCoords.parent;
            if (done(*this, cellCoords.row, cellCoords.num)) {
                endBorder.parent = cell;
                return true;
            }
            for (auto &nextCoords: getNeighbours(*this, cellCoords.row, cellCoords.num)) {
                auto next = getCell(nextCoords.row, nextCoords.num);
                if (!next->visited && next->getType() == color) {
                    nextCoords.parent = cell;
                    nexts.pushBack(nextCoords);
                }
            }
        }
        return false;
    }

    void Board::generateCellsAndConnections() {
        size_t lastRowIndex = 2 * mSize - 2;
        for (auto i = 0u; i < mSize - 1; ++i) {
            auto rowSize = i + 1;
            mBoard[i] = new RowType(rowSize);
            for (auto cellNum = 0u; cellNum < rowSize; ++cellNum) {
                mBoard[i]->at(cellNum) = new Cell;
            }
            mBoard[lastRowIndex - i] = new RowType(rowSize);
            for (auto cellNum = 0u; cellNum < rowSize; ++cellNum) {
                mBoard[lastRowIndex - i]->at(cellNum) = new Cell;
            }
        }
        mBoard[mSize - 1] = new RowType(mSize);
        for (auto cellNum = 0u; cellNum < mSize; ++cellNum) {
            mBoard[mSize - 1]->at(cellNum) = new Cell;
        }

        for (auto row = 0u; row < mSize; ++row) {
            auto cellsInRow = row + 1;
            for (auto i = 0u; i < cellsInRow; ++i) {
                auto *cell = mBoard[row]->at(i);
                if (i == 0u) {
                    createConnection(&mRedBoarderLeft, cell);
                }
                if (i == cellsInRow - 1) {
                    createConnection(&mBlueBoarderRight, cell);
                }
                if (row > 0u) {
                    if (i > 0u) {
                        createConnection(mBoard[row - 1]->at(i - 1), cell);
                    }
                    if (i < cellsInRow - 1) {
                        createConnection(mBoard[row - 1]->at(i), cell);
                    }

                }
                if (row > 1u && i > 0u && i < cellsInRow - 1) {
                    createConnection(mBoard[row - 2]->at(i - 1), cell);
                }
            }
        }
        createConnection(&mBlueBoarderLeft, mBoard[mSize - 1]->at(0));
        createConnection(&mRedBoarderRight, mBoard[mSize - 1]->at(mSize - 1));
        for (auto row = mSize, cellsInRow = mSize - 1; row < 2 * mSize - 1; ++row, --cellsInRow) {
            for (auto i = 0u; i < cellsInRow; ++i) {
                auto *cell = mBoard[row]->at(i);
                if (i == 0u) {
                    createConnection(&mBlueBoarderLeft, cell);
                }
                createConnection(mBoard[row - 1]->at(i), cell);

                if (i == cellsInRow - 1) {
                    createConnection(&mRedBoarderRight, cell);
                }
                createConnection(mBoard[row - 1]->at(i + 1), cell);

                if (row > mSize) {
                    createConnection(mBoard[row - 2]->at(i + 1), cell);
                }
            }
        }
        if (mSize > 1u) {
            for (auto cellNum = 0u; cellNum < mSize - 1; ++cellNum) {
                createConnection(mBoard[mSize - 2]->at(cellNum), mBoard[mSize]->at(cellNum));
            }
        }
    }

    void Board::clearVisited() {
        for (auto *row: mBoard) {
            for (auto *cell: *row) {
                cell->visited = false;
            }
        }
    }

    void Board::visitParents(const Cell *lastChild, const Cell *greatParent) {
        Cell *cell = lastChild->parent;
        while (cell != greatParent) {
            cell->visited = true;
            cell = cell->parent;
        }
    }

} // board