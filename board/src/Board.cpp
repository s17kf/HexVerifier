//
// Created by stefan on 01/04/24.
//

#include "Board.h"
#include "DoneVerifiers.h"
#include "WinVerifiers.h"


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
        RedDoneVerifier doneVerifier(*this);
        RedNeighboursGenerator neighboursGenerator(*this);
        return dfs(nexts, Cell::Type::red, mRedBoarderRight, doneVerifier, neighboursGenerator);
    }

    bool Board::isBlueWin() {
        List<CellCoords> nexts;
        for (size_t row = 0u; row < mSize; ++row) {
            if (mBoard[row]->last()->getType() == Cell::Type::blue)
                nexts.pushBack({row, mBoard[row]->size() - 1, &mBlueBoarderRight, CellCoords::Direction::left});
        }
        BlueDoneVerifier doneVerifier(*this);
        BlueNeighboursGenerator neighboursGenerator(*this);
        return dfs(nexts, Cell::Type::blue, mBlueBoarderLeft, doneVerifier, neighboursGenerator);
    }

    bool Board::canRedWinInNMoves(size_t n) {
        if (!isBoardCorrect() || !enoughEmptyCells(Cell::Type::red, n) || isGameWonBySomeone()) {
            return false;
        }
        List<CellCoords> emptyCellsCoords;
        fillEmptyCellsCoordsList(emptyCellsCoords);
        RedWinVerifier winVerifier(*this);
        return canWin(n, Cell::Type::red, emptyCellsCoords, winVerifier);
    }

    bool Board::canBlueWinInNMoves(size_t n) {
        if (!isBoardCorrect() || !enoughEmptyCells(Cell::Type::blue, n) || isGameWonBySomeone()) {
            return false;
        }
        List<CellCoords> emptyCellsCoords;
        fillEmptyCellsCoordsList(emptyCellsCoords);
        BlueWinVerifier winVerifier(*this);
        return canWin(n, Cell::Type::blue, emptyCellsCoords, winVerifier);
    }

    bool Board::canRedWinInNMovesWithPerfectOpponent(size_t n) {
        CellType color = Cell::Type::red;
        if (!isBoardCorrect() || !enoughEmptyCells(color, n) || isGameWonBySomeone()) {
            return false;
        }
        // calculate distances for empty cells
        List<CellCoords> nexts;
        for (size_t row = 0u; row < mSize; ++row) {
            nexts.pushBack({row, 0, &mRedBoarderLeft});
        }
        for (size_t row = mSize - 1; row < mBoard.size(); ++row) {
            nexts.pushBack({row, mBoard[row]->size() - 1, &mRedBoarderLeft});
        }
        RedNeighboursGenerator neighboursGenerator(*this);
        while (!nexts.empty()) {
            auto coords = nexts.popFront();
            auto cell = getCell(coords);
            cell->visited = true;
            if (cell->getType() != color && cell->getType() != Cell::Type::empty)
                continue;
            if (cell->getType() == color) {
                cell->closestRed = 0;
            } else if (cell->getType() == Cell::Type::empty) {
                cell->closestRed = cell->parent->closestRed + 1;
            }
            neighboursGenerator.fill(nexts, coords, true);
        }
        clearVisited();

        List<CellCoords> emptyCellsToPlay;
        CellRedDistanceEvaluator cellRedDistanceEvaluator(n);
        fillEmptyCellsCoordsList(emptyCellsToPlay, &cellRedDistanceEvaluator);


        return false;
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
                       const WinVerifier &winVerifier) {
        if (movesLeft == 1) {
            for (auto &cellCoords: emptyCellsCoords) {
                auto *cell = getCell(cellCoords.row, cellCoords.num);
                cell->setType(color);
                if (winVerifier()) {
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
            if (winVerifier()) {
                clearVisited();
                cell->setType(Cell::Type::empty);
                emptyCellsCoords.pushBack(cellCoords);
                continue;
            }
            clearVisited();
            if (canWin(movesLeft - 1, color, emptyCellsCoords, winVerifier)) {
                cell->setType(Cell::Type::empty);
                return true;
            }
            cell->setType(Cell::Type::empty);
        }
        return false;
    }

    bool Board::isGameWonBySomeone() {
        if (RedWinVerifier(*this)()) {
            clearVisited();
            return true;
        }
        clearVisited();
        if (BlueWinVerifier(*this)()) {
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
                    const DoneVerifier &doneVerifier, const NeighboursGenerator &neighboursGenerator) {
        for (const auto nextCoords: nexts) {
            auto *cell = getCell(nextCoords.row, nextCoords.num);
            if (cell->visited || cell->getType() != color)
                continue;
            if (dfs(nextCoords, color, endBorder, doneVerifier, neighboursGenerator)) {
                clearVisited();
                return true;
            }
        }
        clearVisited();
        return false;
    }

    bool Board::dfs(const CellCoords &cellCoords, Board::CellType color, Cell &endBorder,
                    const DoneVerifier &doneVerifier, const NeighboursGenerator &neighboursGenerator) {
        auto *cell = getCell(cellCoords.row, cellCoords.num);
        cell->parent = cellCoords.parent;
        cell->visited = true;
        if (doneVerifier(cellCoords)) {
            endBorder.parent = cell;
            return true;
        }
        for (auto &nextCoords: neighboursGenerator.get(cellCoords, false)) {
            if (dfs(nextCoords, color, endBorder, doneVerifier, neighboursGenerator))
                return true;
        }
        return false;
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

    void Board::fillEmptyCellsCoordsList(List <CellCoords> &emptyCellsCoords,
                                         const Board::CellDistanceEvaluator *evaluator) const {
        for (size_t row = 0u; row < mBoard.size(); ++row) {
            for (size_t num = 0u; num < mBoard[row]->size(); ++num) {
                auto *cell = getCell(row, num);
                if (cell->getType() == Cell::Type::empty)
                    if (!evaluator || (*evaluator)(cell))
                        emptyCellsCoords.pushBack({row, num});
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