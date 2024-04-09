//
// Created by stefan on 02/04/24.
//

#include "HexVerifier.h"

#include <charconv>

#include "Board.h"
#include "BoardParser.h"
#include "String.h"

using utils::String;

namespace board {

    void HexVerifier::handleQuery(utils::String *query, Board *board) {
        if (*query == "BOARD_SIZE") {
            printf("%lu\n", board->size());
            return;
        }
        if (*query == "PAWNS_NUMBER") {
            auto pawnsNumber = board->getColorCount(Cell::Type::red) + board->getColorCount(Cell::Type::blue);
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
        if (*query == "IS_GAME_OVER") {
            if (!board->isBoardCorrect()) {
                printf("NO\n");
                return;
            }
            if (board->isGameWonByRed()) {
                printf("YES RED\n");
                return;
            }
            if (board->isGameWonByBlue()) {
                printf("YES BLUE\n");
                return;
            }
            printf("NO\n");
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
        auto splittedQuery = query->split('_');
        auto wordIt = splittedQuery.begin();
        if(*(wordIt++) != "CAN") {
            printf("%s: query is NOT SUPPORTED!\n", query->c_str());
            return;
        }
        String &color = *wordIt++;
        ++wordIt; // skip 'WIN'
        ++wordIt; // skip 'IN'
        String &movesStr = *wordIt++;
        int moves;
        std::from_chars(movesStr.c_str(), movesStr.c_str() + movesStr.size(), moves);
        ++wordIt; // skip 'MOVE(S)'
        ++wordIt; // skip 'WITH'
        String &opponentType = *wordIt;
        if(opponentType == "NAIVE") {
            if(color == "RED"){
                if (board->canRedWinInNMovesWithNaive(moves))
                    printf("YES\n");
                else
                    printf("NO\n");
                return;
            }

            if(color == "BLUE"){
                if (board->canBlueWinInNMovesWithNaive(moves))
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

        String *lastLine = inputReader.getLine();
        while (!inputReader.eof() && *lastLine != boardParser.BOARD_DELIMITER) {
            if (lastLine->size() == 0) {
                delete lastLine;
                lastLine = inputReader.getLine();
                continue;
            }
            handleQuery(lastLine, board);
            printf("\n");
            delete lastLine;
            lastLine = inputReader.getLine();
        }
        printf("\n");
        delete lastLine;
        delete board;
    }


    void HexVerifier::handleInput(utils::InputReader &inputReader) {
        while (!inputReader.eof()) {
            handleBoard(inputReader);
        }
    }

} // board