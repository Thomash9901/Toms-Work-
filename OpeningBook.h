#ifndef OPENINGBOOK_H
#define OPENINGBOOK_H

#include <string>
#include <vector>
#include <map>
#include "chessBoard.h"
#include "MoveScore.h"

extern std::map<std::string, std::vector<std::pair<int, int>>> originalOpeningBook;
extern std::map<std::string, std::vector<std::pair<int, int>>> aggregatedOpeningBook;

void loadOpeningBookFromFiles(const std::vector<std::string>& filenames);
void loadAggregatedOpeningBook(const std::string &filename);

#endif // OPENINGBOOK_H
