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
        mRedBoarderRight.closestRed = 0;
        mRedBoarderLeft.closestRed = 0;
        mBlueBoarderRight.closestBlue = 0;
        mBlueBoarderLeft.closestBlue = 0;
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
        mRedBoarderRight.closestRed = 0;
        mRedBoarderLeft.closestRed = 0;
        mBlueBoarderRight.closestBlue = 0;
        mBlueBoarderLeft.closestBlue = 0;
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
            case Cell::Type::empty:
                return mSize * mSize - redCellsCount - blueCellsCount;
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

    bool Board::isBoardPossible() {
        if (!isBoardCorrect())
            return false;
        auto redPawns = getColorCount(Cell::Type::red);
        auto bluePawns = getColorCount(Cell::Type::blue);
        if (isRedWin()) {
            if (bluePawns == redPawns) {
                return false;
            }
            clearVisited();
            visitParents(&mRedBoarderRight, &mRedBoarderLeft);
            return !isRedWin();
        }
        if (isBlueWin()) {
            if (bluePawns < redPawns) {
                return false;
            }
            clearVisited();
            visitParents(&mBlueBoarderLeft, &mBlueBoarderRight);
            return !isBlueWin();
        }
        return true;
    }

    bool Board::isRedWin() {
        List<CellCoords> nexts;
        for (size_t row = 0u; row < mSize; ++row) {
            if (mBoard[row]->at(0)->getType() == Cell::Type::red)
                nexts.pushBack({row, 0, &mRedBoarderLeft, CellCoords::Direction::right});
        }
        return dfs(nexts, Cell::Type::red, mRedBoarderRight, Board::doneForRed, Board::getNeighboursForRed);
    }

    bool Board::isBlueWin() {
        List<CellCoords> nexts;
        for (size_t row = 0u; row < mSize; ++row) {
            if (mBoard[row]->last()->getType() == Cell::Type::blue)
                nexts.pushBack({row, mBoard[row]->size() - 1, &mBlueBoarderRight, CellCoords::Direction::left});
        }
        return dfs(nexts, Cell::Type::blue, mBlueBoarderLeft, Board::doneForBlue, Board::getNeighboursForBlue);
    }

    bool Board::canRedWinInNMoves(size_t n) {
        if (!isBoardCorrect() || !enoughEmptyCells(Cell::Type::red, n) || isGameWonBySomeone()) {
            return false;
        }
        List<CellCoords> emptyCellsCoords;
        fillEmptyCellsCoordsList(emptyCellsCoords);
        return canWin(n, Cell::Type::red, emptyCellsCoords, Board::isRedWinBfs);
    }

    bool Board::canBlueWinInNMoves(size_t n) {
        if (!isBoardCorrect() || !enoughEmptyCells(Cell::Type::blue, n) || isGameWonBySomeone()) {
            return false;
        }
        List<CellCoords> emptyCellsCoords;
        fillEmptyCellsCoordsList(emptyCellsCoords);
        return canWin(n, Cell::Type::blue, emptyCellsCoords, Board::isBlueWinBfs);
    }

    bool Board::canRedWinInNMovesWithPerfectOpponent(size_t n) {
        CellType color = Cell::Type::red;
        if (!isBoardCorrect() || !enoughEmptyCells(color, n) || isGameWonBySomeone()) {
            return false;
        }
        List<CellCoords> nexts;
        for (size_t row = 0u; row < mSize; ++row) {
            nexts.pushBack({row, 0, &mRedBoarderLeft});
        }
        for (size_t row = mSize - 1; row < mBoard.size(); ++row) {
            nexts.pushBack({row, mBoard[row]->size() - 1, &mRedBoarderLeft});
        }
        while (!nexts.empty()) {
            auto coords = nexts.popFront();
            auto cell = getCell(coords);
            cell->visited = true;

            break;
        }
        clearVisited();
        return false;
    }

    void Board::fillEmptyCellsCoordsList(data_structures::List<CellCoords> &emptyCellsCoords) const {
        for (size_t row = 0u; row < mBoard.size(); ++row) {
            for (size_t num = 0u; num < mBoard[row]->size(); ++num) {
                if (mBoard[row]->at(num)->getType() == Cell::Type::empty)
                    emptyCellsCoords.pushBack({row, num});
            }
        }
    }

    bool Board::enoughEmptyCells(CellType playerToCheck, size_t neededMoves) const {
        size_t neededEmptyCells;
        switch (playerToCheck) {
            case Cell::Type::red:
                neededEmptyCells = redCellsCount > blueCellsCount ? 2 * neededMoves : 2 * neededMoves - 1;
                break;
            case Cell::Type::blue:
                neededEmptyCells = redCellsCount > blueCellsCount ? 2 * neededMoves - 1 : 2 * neededMoves;
                break;
            default:
                throw std::invalid_argument("Can check empty cells count for red or blue only");
        }
        return getColorCount(Cell::Type::empty) >= neededEmptyCells;
    }

    bool Board::canWin(size_t movesLeft, CellType color, List <CellCoords> &emptyCellsCoords,
                       const std::function<bool(Board &)> &isWin) {
        if (movesLeft == 1) {
            for (auto &cellCoords: emptyCellsCoords) {
                auto *cell = getCell(cellCoords.row, cellCoords.num);
                cell->setType(color);
                if (isWin(*this)) {
                    cell->setType(Cell::Type::empty);
                    clearVisited();
                    return true;
                }
                clearVisited();
                cell->setType(Cell::Type::empty);
            }
            return false;
        }
        for (size_t loopsLeft = emptyCellsCoords.size();
             loopsLeft > 0 && emptyCellsCoords.size() >= movesLeft; --loopsLeft) {
            auto cellCoords = emptyCellsCoords.popFront();
            auto *cell = getCell(cellCoords.row, cellCoords.num);
            cell->setType(color);
            if (isWin(*this)) {
                clearVisited();
                cell->setType(Cell::Type::empty);
                emptyCellsCoords.pushBack(cellCoords);
                continue;
            }
            clearVisited();
            if (canWin(movesLeft - 1, color, emptyCellsCoords, isWin)) {
                cell->setType(Cell::Type::empty);
                return true;
            }
            cell->setType(Cell::Type::empty);
        }
        return false;
    }

    bool Board::isGameWonBySomeone() {
        if (isRedWinBfs(*this)) {
            clearVisited();
            return true;
        }
        clearVisited();
        if (isBlueWinBfs(*this)) {
            clearVisited();
            return true;
        }
        clearVisited();
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

    bool Board::dfs(const List <CellCoords> &nexts, Board::CellType color, Cell &endBorder,
                    const std::function<bool(const Board &, size_t, size_t)> &done,
                    const std::function<List<CellCoords>(const Board &, const CellCoords &cellCoords)> &getNeighbours) {
        for (const auto nextCoords: nexts) {
            auto *cell = getCell(nextCoords.row, nextCoords.num);
            if (cell->visited || cell->getType() != color)
                continue;
            if (dfs(nextCoords, color, endBorder, done, getNeighbours)) {
                clearVisited();
                return true;
            }
        }
        clearVisited();
        return false;
    }

    bool Board::dfs(const Board::CellCoords &cellCoords, Board::CellType color, Cell &endBorder,
                    const std::function<bool(const Board &, size_t, size_t)> &done,
                    const std::function<List<CellCoords>(const Board &, const CellCoords &cellCoords)> &getNeighbours) {
        auto *cell = getCell(cellCoords.row, cellCoords.num);
        cell->parent = cellCoords.parent;
        cell->visited = true;
        if (done(*this, cellCoords.row, cellCoords.num)) {
            endBorder.parent = cell;
            return true;
        }
        for (auto &nextCoords: getNeighbours(*this, cellCoords)) {
            if (dfs(nextCoords, color, endBorder, done, getNeighbours))
                return true;
        }
        return false;
    }

    bool Board::doneForBlue(const Board &board, size_t row, size_t num) {
        return row >= board.mSize - 1 && num == 0;
    }

    bool Board::doneForRed(const Board &board, size_t row, size_t num) {
        return row >= board.mSize - 1 && num == board.mBoard[row]->size() - 1;
    }

    List <Board::CellCoords> Board::getNeighboursForBlue(const Board &board, const CellCoords &cellCoords) {
        auto row = cellCoords.row;
        auto num = cellCoords.num;
        auto *cell = board.getCell(cellCoords);
        List<CellCoords> neighbours;
        if (cellCoords.direction != CellCoords::Direction::right)
            board.addNeighbourAbove(row, num, cell, CellType::blue, neighbours);
        board.addNeighbourOnLeft(row, num, cell, CellType::blue, neighbours, true);
        board.addNeighbourBelow(row, num, cell, CellType::blue, neighbours);
        board.addNeighbourOnRight(row, num, cell, CellType::blue, neighbours, false);
        if (cellCoords.direction == CellCoords::Direction::right)
            board.addNeighbourAbove(row, num, cell, CellType::blue, neighbours);
        return neighbours;
    }

    List <Board::CellCoords> Board::getNeighboursForRed(const Board &board, const CellCoords &cellCoords) {
        auto row = cellCoords.row;
        auto num = cellCoords.num;
        auto *cell = board.getCell(cellCoords);
        List<CellCoords> neighbours;
        if (cellCoords.direction != CellCoords::Direction::left)
            board.addNeighbourAbove(row, num, cell, CellType::red, neighbours);
        board.addNeighbourOnRight(row, num, cell, CellType::red, neighbours, true);
        board.addNeighbourBelow(row, num, cell, CellType::red, neighbours);
        board.addNeighbourOnLeft(row, num, cell, CellType::red, neighbours, false);
        if (cellCoords.direction == CellCoords::Direction::left)
            board.addNeighbourAbove(row, num, cell, CellType::red, neighbours);
        return neighbours;
    }

    void Board::addNeighbourAbove(
            size_t row, size_t num, Cell *cell, CellType color, List <CellCoords> &neighbours) const {
        if (row > 1) {
            if (row == mSize) {
                addToListIfNotVisited(row - 2, num, neighbours, cell, CellCoords::Direction::up, color);
            } else {
                if (row < mSize) {
                    if (num > 0 && num < mBoard[row]->size() - 1)
                        addToListIfNotVisited(row - 2, num - 1, neighbours, cell, CellCoords::Direction::up, color);;
                } else {
                    addToListIfNotVisited(row - 2, num + 1, neighbours, cell, CellCoords::Direction::up, color);;
                }
            }
        }
    }

    void Board::addNeighbourBelow(
            size_t row, size_t num, Cell *cell, CellType color, List <CellCoords> &neighbours) const {
        if (row < mBoard.size() - 2) {
            if (row == mSize - 2) {
                addToListIfNotVisited(row + 2, num, neighbours, cell, CellCoords::Direction::down, color);;
            } else {
                if (row < mSize - 1) {
                    addToListIfNotVisited(row + 2, num + 1, neighbours, cell, CellCoords::Direction::down, color);;
                } else {
                    if (num > 0 && num < mBoard[row]->size() - 1)
                        addToListIfNotVisited(row + 2, num - 1, neighbours, cell, CellCoords::Direction::down, color);;
                }
            }
        }
    }

    void Board::addNeighbourOnLeft(size_t row, size_t num, Cell *cell, CellType color, List <CellCoords> &neighbours,
                                   bool topFirst) const {
        if (row == mSize - 1) {
            if (num > 0) {
                if (!topFirst)
                    addToListIfNotVisited(row + 1, num - 1, neighbours, cell, CellCoords::Direction::left, color);;
                addToListIfNotVisited(row - 1, num - 1, neighbours, cell, CellCoords::Direction::left, color);;
                if (topFirst)
                    addToListIfNotVisited(row + 1, num - 1, neighbours, cell, CellCoords::Direction::left, color);;
            }
        } else {
            if (row < mSize) {
                if (!topFirst)
                    addToListIfNotVisited(row + 1, num, neighbours, cell, CellCoords::Direction::left, color);;
                if (num > 0)
                    addToListIfNotVisited(row - 1, num - 1, neighbours, cell, CellCoords::Direction::left, color);;
                if (topFirst)
                    addToListIfNotVisited(row + 1, num, neighbours, cell, CellCoords::Direction::left, color);;
            } else {
                if (!topFirst && num > 0)
                    addToListIfNotVisited(row + 1, num - 1, neighbours, cell, CellCoords::Direction::left, color);;
                addToListIfNotVisited(row - 1, num, neighbours, cell, CellCoords::Direction::left, color);;
                if (topFirst && num > 0)
                    addToListIfNotVisited(row + 1, num - 1, neighbours, cell, CellCoords::Direction::left, color);;
            }
        }
    }

    void Board::addNeighbourOnRight(size_t row, size_t num, Cell *cell, CellType color, List <CellCoords> &neighbours,
                                    bool topFirst) const {
        if (row == mSize - 1) {
            if (num < mSize - 1) {
                if (!topFirst)
                    addToListIfNotVisited(row + 1, num, neighbours, cell, CellCoords::Direction::right, color);;
                addToListIfNotVisited(row - 1, num, neighbours, cell, CellCoords::Direction::right, color);;
                if (topFirst)
                    addToListIfNotVisited(row + 1, num, neighbours, cell, CellCoords::Direction::right, color);;
            }
        } else {
            if (row < mSize) {
                if (!topFirst)
                    addToListIfNotVisited(row + 1, num + 1, neighbours, cell, CellCoords::Direction::right, color);;
                if (row > 0 && num < mBoard[row]->size() - 1)
                    addToListIfNotVisited(row - 1, num, neighbours, cell, CellCoords::Direction::right, color);;
                if (topFirst)
                    addToListIfNotVisited(row + 1, num + 1, neighbours, cell, CellCoords::Direction::right, color);;
            } else {
                if (!topFirst && row < mBoard.size() - 1 && num < mBoard[row]->size() - 1)
                    addToListIfNotVisited(row + 1, num, neighbours, cell, CellCoords::Direction::right, color);;
                addToListIfNotVisited(row - 1, num + 1, neighbours, cell, CellCoords::Direction::right, color);;
                if (topFirst && row < mBoard.size() - 1 && num < mBoard[row]->size() - 1)
                    addToListIfNotVisited(row + 1, num, neighbours, cell, CellCoords::Direction::right, color);;
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

    void Board::addToListIfNotVisited(size_t row, size_t num, List <CellCoords> &list, Cell *parent,
                                      CellCoords::Direction direction, CellType color) const {
        if (!visited(row, num) && getCell(row, num)->getType() == color)
            list.pushBack({row, num, parent, direction});
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