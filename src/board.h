#pragma once
#include <cmath>
#include <vector>
#include <algorithm>

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
		std::vector<int> heldPieceMoves;
		std::vector<std::vector<int>> numSquaresToEdge;

		// just crashed my program by not setting it to 0
		// not sure how it did not happen in the last like 12038219032183 build tests
		int heldPiece = 0;

		int colorToMove;

		BoardData ExportData();

		void GenerateHeldPieceMoves(int startSquare);	
		void GeneratePossibleMoves();
		
		int GetIndexRank(int index);
		int GetIndexFile(int index);

		void ClearPossibleMoveData();
		void ClearHeldPieceMoveData();

		bool IsMoveValid(int targetSquare);
		bool AreTwoSquaresInRange(int startSquare, int targetSquare, int range);

		// contains the previous square the moved piece was at and the next square
		// the piece moves to
		int lastSquare = -1;
		int nextSquare = -1;

	private:
		void CalculateNumSquaresToEdge();
		
		void GenerateSlidingPieceMoves(int startSquare, int piece);
		void GenerateKnightMoves(int startSquare);
		void GenerateKingMoves(int startSquare);

		std::string startingFen = "nrqb/8/8/8/8/8/8/KRBQ w KQkq - 0 1";
		int directionOffsets[8] = { 8, -8, 1, -1, 7, -7, 9, -9 };
};
