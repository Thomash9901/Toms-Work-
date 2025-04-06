#include "OpeningBook.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <random>

std::map<std::string, std::vector<std::pair<int, int>>> originalOpeningBook;
std::map<std::string, std::vector<std::pair<int, int>>> aggregatedOpeningBook;

void loadOpeningBookFromFiles(const std::vector<std::string>& filenames) {
    for (const auto& filename : filenames)
    {
        std::ifstream file(filename);

        std::string line;
        bool isHeader = true;
        while (std::getline(file, line)) {
            if (isHeader)
            {
                isHeader = false;
                continue;
            }
            if (line.empty()) continue;
            std::stringstream ss(line);
            std::string eco, name, pgn, uci, epd;
            if (!std::getline(ss, eco, '\t') || !std::getline(ss, name, '\t') || !std::getline(ss, pgn, '\t') ||
                !std::getline(ss, uci, '\t') || !std::getline(ss, epd, '\t')) {
                continue;
            }
            std::vector<std::pair<int, int>> movesForFen;
            std::stringstream moveStream(uci);
            std::string moveStr;
            while (moveStream >> moveStr)
            {
                if (moveStr.length() >= 4)
                {
                    int fromIndex = ChessBoard::chessInputToIndex(moveStr.substr(0, 2));
                    int toIndex = ChessBoard::chessInputToIndex(moveStr.substr(2, 2));
                    if (fromIndex != -1 && toIndex != -1)
                    {
                        movesForFen.push_back({fromIndex, toIndex});
                    }
                }
            }
            if (!epd.empty() && !movesForFen.empty())
            {
                originalOpeningBook[epd] = movesForFen;
            }
        }
        file.close();
    }
}

void loadAggregatedOpeningBook(const std::string &filename) {
    std::ifstream file(filename);
    std::string line;
    while (std::getline(file, line))
    {
        if (line.empty()) continue;
        std::stringstream ss(line);
        std::string epd, candidateMoves;
        std::vector<std::pair<int, int>> moves;
        std::istringstream iss(candidateMoves);
        std::string moveToken;
        while (iss >> moveToken)
        {
            std::size_t colonPos = moveToken.find(':');
            if (colonPos != std::string::npos && colonPos >= 4)
            {
                std::string moveStr = moveToken.substr(0, 4);
                int fromIndex = ChessBoard::chessInputToIndex(moveStr.substr(0, 2));
                int toIndex = ChessBoard::chessInputToIndex(moveStr.substr(2, 2));
                if (fromIndex != -1 && toIndex != -1) {
                    moves.push_back({fromIndex, toIndex});
                }
            }
        }
        if (!epd.empty() && !moves.empty())
        {
            aggregatedOpeningBook[epd] = moves;
        }
    }
    file.close();
}
