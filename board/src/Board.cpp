//
// Created by stefan on 01/04/24.
//

#include "Board.h"

using data_structures::Vector;

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
        if (redPawns < bluePawns || redPawns > bluePawns + 1){
            return false;
        }
        return true;
    }

    bool Board::isBoardPossible() const {
        if (!isBoardCorrect())
            return false;
        auto redPawns = getColorCount(Cell::Type::red);
        auto bluePawns = getColorCount(Cell::Type::blue);
        if (bluePawns == redPawns && isRedWin())
            return false;
        if (bluePawns < redPawns && isBlueWin())
            return false;
        return true;
    }

    bool Board::isRedWin() const {
        Board copy(*this);
        return copy.bfs(&copy.mRedBoarderRight, &copy.mRedBoarderLeft, CellType::red);
    }

    bool Board::isBlueWin() const {
        Board copy(*this);
        return copy.bfs(&copy.mBlueBoarderLeft, &copy.mBlueBoarderRight, CellType::blue);
    }

    bool Board::bfs(Cell *start, Cell *end, Board::CellType acceptedType) {
        data_structures::List<Cell *> nexts;
        for (auto *cell: start->getNeighbours()) {
            if (cell->getType() == acceptedType) {
                nexts.pushBack(cell);
            }
        }
        while (!nexts.empty()) {
            auto *cell = nexts.popFront();
            if (cell->visited)
                continue;
            cell->visited = true;
            for (auto *next: cell->getNeighbours()) {
                if (next == end)
                    return true;
                if (!next->visited && next->getType() == acceptedType) {
                    nexts.pushBack(next);
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

} // board