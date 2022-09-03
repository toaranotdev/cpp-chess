#pragma once
#include <cmath>
#include <vector>

#include "piece.h"
#include "fen_util.h"

struct Move {
	int startSquare;
	int targetSquare;
};

class Board {
	public:
		Board();
		int squares[64] = { 0 }; // initialize them all with 0

		std::vector<Move> possibleMoves;
		std::vector<std::vector<int>> numSquaresToEdge;

		int heldPiece;

	private:
		void CalculateNumSquaresToEdge();
		
		void GeneratePossibleMoves();
		void GenerateSlidingPieceMoves(int startSquare, int piece);

		std::string startingFen = "nrqb/8/8/8/8/8/8/RBQ w KQkq - 0 1";
		int directionOffsets[8] = { 8, -8, 1, -1, 7, -7, 9, -9 };

};
