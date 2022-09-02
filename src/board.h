#pragma once
#include <cmath>

#include "piece.h"
#include "fen_util.h"

class Board {
	public:
		Board();
		int squares[64] = { 0 }; // initialize them all with 0

	private:
		std::string startingFen = "nrqb/8/8/8/8/8/8/RBQ w KQkq - 0 1";
};
