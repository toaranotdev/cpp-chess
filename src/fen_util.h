#include <iostream>
#include <string>
#include <map>
#include <utility>

#include "piece.h"

namespace FenUtil {
	void LoadFen(int* square, std::string fenString);

	// apparently you cannot use double quotes for chars otherwise shit happens
	extern std::map<char, int> pieceMap;
};
