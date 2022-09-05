#include <iostream>
#include <string>
#include <map>
#include <utility>

#include "piece.h"

// struct that represent board data
struct BoardData {
	int* squares;
	int* colorToMove;
};

namespace FenUtil {
	void LoadFen(BoardData data, std::string fenString);

	extern std::map<char, int> pieceMap;
};
