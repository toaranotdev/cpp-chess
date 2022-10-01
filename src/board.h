#pragma once

#include <cmath>
#include <vector>
#include <algorithm>
#include <list>
#include <iterator>

#include "fen_util.h"

enum Flag {
	MOVE,
	PROMOTION,
	DOUBLE_PUSH,
	EN_PASSANT,
	CASTLE_KING_SIDE,
	CASTLE_QUEEN_SIDE
};

struct Move {
	int startSquare;
	int targetSquare;
	int flag;

	bool operator==(const Move& move) const;
};

class Board {
	public:
		Board();
		int squares[64] = { 0 }; // initialize them all with 0

		// containing all the possible moves for the side that's actually moving
		std::list<Move> possibleMoves;
		// we can't actually reuse this to save time generating
		// the next moves since the position can change drastically and it becomes
		// a headache which is a nono
		std::list<Move> opponentsPossibleMoves;
		// containing only the target squares that the current holding piece can move to
		std::vector<int> heldPieceMoves;
		// like the name suggest xd
		std::vector<std::vector<int>> numSquaresToEdge;

		// just crashed my program by not setting it to 0
		// not sure how it did not happen in the last like 12038219032183 build tests
		int heldPiece = 0;

		int colorToMove;

		// export the current board array and the color to move
		// for the fen utility to process
		BoardData ExportData();

		void GenerateHeldPieceMoves(int startSquare);	
		void CalculateMoves();
		
		int GetIndexRank(int index);
		int GetIndexFile(int index);
		int GetDirectionOffset(int startSquare, int targetSquare);

		void ClearMoveData();
		void ClearHeldPieceMoveData();

		bool IsMoveValid(int targetSquare);
		bool AreTwoSquaresInRange(int startSquare, int targetSquare, int range);
		bool IsSquareUnderEnemyControl(int startSquare, int targetSquare);
		bool AreTwoSquaresOnSameLine(int startSquare, int targetSquare, int directionOffset);

		void FilterMove();

		int GetMoveFlag(int startSquare, int targetSquare);

		// contains the previous square the moved piece was at and the next square
		// the piece moves to
		int lastSquare = -1;
		int nextSquare = -1;

		// the file that was marked as en passant-able
		int enPassantFile = -1;

		int whiteCastleRights = 0b1111;
		int blackCastleRights = 0b1111;

		int kingSideCastleMask = 0b1100;
		int queenSideCastleMask = 0b0011;
	private:
		void CalculateNumSquaresToEdge();

		std::vector<Move> GenerateSlidingPieceMoves(int startSquare, int piece);
		std::vector<Move> GenerateKnightMoves(int startSquare);
		std::vector<Move> GenerateKingMoves(int startSquare, int color);
		std::vector<Move> GeneratePawnMoves(int startSquare, int color);

		// the square our current king occupies
		int friendlyKingSquare = 0;

		std::vector<int> opponentAttackedSquares;
		std::vector<Move> opponentLinesTowardsKing;

		std::string startingFen = "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1";
		int directionOffsets[8] = { 8, -8, 1, -1, 7, -7, 9, -9 };

		std::map<int, int> directionIndexMap = {
			{ 8, 0 }, { -8, 1 }, { 1, 2 }, { -1, 3 },
			{ 7, 4 }, { -7, 5 }, { 9, 6 }, { -9, 7 }
		};
};
