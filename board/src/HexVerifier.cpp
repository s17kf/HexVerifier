//
// Created by stefan on 02/04/24.
//

#include "HexVerifier.h"

#include <charconv>

#include "Board.h"
#include "BoardParser.h"
#include "String.h"

using utils::String;
using data_structures::List;

namespace board {

    void HexVerifier::handleQuery(const std::string *query, Board *board, DistancesKeeper *&distancesKeeper) {
        if (*query == "BOARD_SIZE") {
            printf("%lu\n", board->size());
            return;
        }
        if (*query == "PAWNS_NUMBER") {
            auto pawnsNumber = board->getColorCount(Color::red) + board->getColorCount(Color::blue);
            printf("%lu\n", pawnsNumber);
            return;
        }
        if (*query == "IS_BOARD_CORRECT") {
            if (board->isBoardCorrect()) {
                printf("YES\n");
            } else {
                printf("NO\n");
            }
            return;
        }
        if (*query == "IS_BOARD_POSSIBLE") {
            if (board->isBoardPossible()) {
                printf("YES\n");
            } else {
                printf("NO\n");
            }
            return;
        }
        if (!distancesKeeper)
            distancesKeeper = new DistancesKeeper(*board);
        if (*query == "IS_GAME_OVER") {
            if (!board->isBoardCorrect()) {
                printf("NO\n");
                return;
            }
            if (board->isGameWonByRed(*distancesKeeper)) {
                printf("YES RED\n");
                return;
            }
            if (board->isGameWonByBlue(*distancesKeeper)) {
                printf("YES BLUE\n");
                return;
            }
            printf("NO\n");
            return;
        }
        List<std::string> splittedQuery;
        utils::String::split(query, splittedQuery, '_');
        auto wordIt = splittedQuery.begin();
        if (*(wordIt++) != "CAN") {
            printf("%s: query is NOT SUPPORTED!\n", query->c_str());
            return;
        }
        std::string &color = *wordIt++;
        ++wordIt; // skip 'WIN'
        ++wordIt; // skip 'IN'
        int moves = std::stoi(*wordIt++);
        ++wordIt; // skip 'MOVE(S)'
        ++wordIt; // skip 'WITH'
        std::string &opponentType = *wordIt;
        if (opponentType == "NAIVE") {
            if (color == "RED") {
                if (board->canRedWinInNMovesWithNaive(moves, *distancesKeeper))
                    printf("YES\n");
                else
                    printf("NO\n");
                return;
            }
            if (color == "BLUE") {
                if (board->canBlueWinInNMovesWithNaive(moves, *distancesKeeper))
                    printf("YES\n");
                else
                    printf("NO\n");
                return;
            }
        } else {
            if (color == "RED") {
                if (board->canRedWinInNMovesWithPerfect(moves, *distancesKeeper))
                    printf("YES\n");
                else
                    printf("NO\n");
                return;
            } else {
                if (board->canBlueWinInNMovesWithPerfect(moves, *distancesKeeper))
                    printf("YES\n");
                else
                    printf("NO\n");
                return;
            }
        }
        printf("%s: query is not handled yet\n", query->c_str());
    }

    void HexVerifier::handleBoard(utils::InputReader &inputReader) {
        BoardParser boardParser(inputReader);
        Board *board = boardParser.generateBoard();
        if (board == nullptr)
            return;
        DistancesKeeper *distancesKeeper = nullptr;
        std::string *lastLine = inputReader.getLine();
        while (!inputReader.eof() && *lastLine != boardParser.BOARD_DELIMITER) {
            if (lastLine->empty()) {
                delete lastLine;
                lastLine = inputReader.getLine();
                continue;
            }
            handleQuery(lastLine, board, distancesKeeper);
            delete lastLine;
            lastLine = inputReader.getLine();
        }
        printf("\n");
        delete distancesKeeper;
        delete lastLine;
        delete board;
    }

    void HexVerifier::handleInput(utils::InputReader &inputReader) {
        while (!inputReader.eof()) {
            handleBoard(inputReader);
        }
    }

} // board