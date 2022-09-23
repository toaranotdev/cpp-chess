#include "board.h"

Board::Board() {
	BoardData data = this->ExportData();
	FenUtil::LoadFen(data, this->startingFen);
	
	this->CalculateNumSquaresToEdge();
	this->GeneratePossibleMoves();
}

BoardData Board::ExportData() {
	return { this->squares, &(this->colorToMove) };
}

void Board::CalculateNumSquaresToEdge() {
	for (int rank = 0; rank < 8; rank ++) {
		for (int file = 0; file < 8; file ++) {			
            		int numNorth = 7 - rank;
            		int numSouth = rank;
            		int numWest = file;
            		int numEast = 7 - file;

           		this->numSquaresToEdge.push_back({
                		numNorth,
                		numSouth,	
               			numEast,
                		numWest,
               			std::min (numNorth, numWest),
                		std::min (numSouth, numEast),
                		std::min (numNorth, numEast),
                		std::min (numSouth, numWest)
			});
		}
	}
}

void Board::GenerateHeldPieceMoves(int startSquare) {
	for (Move move : this->possibleMoves) {
		if (move.startSquare == startSquare) {
			this->heldPieceMoves.push_back(move.targetSquare);
		}
	}
}
void Board::GeneratePossibleMoves() {
	this->ClearPossibleMoveData();
	for (int i = 0; i < 64; i ++) {
		int piece = Board::squares[i];
		int type = Piece::GetType(piece);

		if (piece && Piece::IsColor(piece, this->colorToMove)) {
			switch (type) {
				case Piece::bishop:
				case Piece::rook:
				case Piece::queen: {
					this->GenerateSlidingPieceMoves(i, piece);
					break;
				}
				case Piece::knight: {
					this->GenerateKnightMoves(i);
					break;
				}
				case Piece::king: {
					this->GenerateKingMoves(i);
					break;
				}
				case Piece::pawn: {
					this->GeneratePawnMoves(i, piece);
				}
				default:
					break;
			}
		}
	}
}

void Board::GenerateSlidingPieceMoves (int startSquare, int piece) {
	int startDirIndex = (Piece::IsType (piece, Piece::bishop)) ? 4 : 0;
    	int endDirIndex = (Piece::IsType (piece, Piece::rook)) ? 4 : 8;

    	for (int directionIndex = startDirIndex; directionIndex < endDirIndex; directionIndex ++) {
		for (int square = 0; square < this->numSquaresToEdge[startSquare][directionIndex]; square ++) {
		
			int targetSquare = startSquare + this->directionOffsets[directionIndex] * (square + 1);
            		int pieceOnTargetSquare = this->squares[targetSquare];

			// target square contains a friendly piece, can't move further
			if (Piece::IsColor(pieceOnTargetSquare, this->colorToMove)) {
				break;
			}

            		this->possibleMoves.push_back ({ startSquare, targetSquare, Flag::MOVE });

			// target square contains enemy piece, can't move further after capturing it
            		if (pieceOnTargetSquare != Piece::none) {
               			break;
            		}
		}
    	}
}

void Board::GenerateKnightMoves(int startSquare) {
	const int knightJumps[] = { 15, 17, -15, -17, 6, 10, -6, -10 };
	
	for (int offset : knightJumps) {
		int targetSquare = startSquare + offset;
			
		// make sure the knights only move two squares
		// on the x, y axis to eliminate indecies that wrapped around the board
		bool isRangeValid = (this->AreTwoSquaresInRange(startSquare, targetSquare, 2) &&
					targetSquare < 64 && targetSquare >= 0);
	
		if (isRangeValid) {
			int pieceOnTargetSquare = this->squares[targetSquare];
			
			if (Piece::IsColor(pieceOnTargetSquare, this->colorToMove))
				continue;
			
			this->possibleMoves.push_back({ startSquare, targetSquare, Flag::MOVE });
		}
	}
}

void Board::GenerateKingMoves (int startSquare) {
	const int kingMoves[] = { 1, -1, 8, -8, 7, -7, 9, -9 };
	for (int o : kingMoves) {
		int targetSquare = startSquare + o;
		bool isValid = (this->AreTwoSquaresInRange(startSquare, targetSquare, 1)) && targetSquare >= 0 && targetSquare < 64;
		
		if (isValid) {
			int pieceOnTargetSquare = this->squares[targetSquare];
			if (Piece::IsColor(pieceOnTargetSquare, this->colorToMove)) 
				continue;

			this->possibleMoves.push_back({ startSquare, targetSquare, Flag::MOVE });
		}
	}
}

void Board::GeneratePawnMoves (int startSquare, int piece) {
	const int pawnMoves[] = { 8, 7, 9 };	
	int color = Piece::GetColor(piece);
	int directionOffset = (color == Piece::white) ? 1 : -1;
	int startRank = this->GetIndexRank(startSquare);

	for (int offset : pawnMoves) {
		// pushes
		if (offset == 8) {
			int targetSquare = startSquare + offset * directionOffset;
			int targetRank = this->GetIndexRank(targetSquare);
			// make sure the pawn only moves 1 square blablabla
			bool isValid = this->AreTwoSquaresInRange(startSquare, targetSquare, 1);
			if (isValid) {
				int pieceOnTargetSquare = this->squares[targetSquare];
				// something is blocking the way, we're not gonna look any
				// further
				if (pieceOnTargetSquare)
					continue;

				int flag = ((Piece::IsColor(piece, Piece::white) && targetRank == 7) ||
						(Piece::IsColor(piece, Piece::black) && targetRank == 0)) ? Flag::PROMOTION : Flag::MOVE;

				this->possibleMoves.push_back({ startSquare, targetSquare, flag });
				
				// double pawn pushes
				if ((Piece::IsColor(piece, Piece::white) && startRank == 1) ||
					(Piece::IsColor(piece, Piece::black) && startRank == 6)) {

					targetSquare = startSquare + offset * directionOffset * 2;
					pieceOnTargetSquare = this->squares[targetSquare];

					if (pieceOnTargetSquare)
						continue;

					this->possibleMoves.push_back({ startSquare, targetSquare, Flag::DOUBLE_PUSH });
				}
			}
		// captures
		} else if (offset == 7 || offset == 9) {
			int targetSquare = startSquare + offset * directionOffset;
			bool isValid = this->AreTwoSquaresInRange(startSquare, targetSquare, 1);
			
			if (isValid) {
				int targetFile = this->GetIndexFile(targetSquare);
				int pieceOnTargetSquare = this->squares[targetSquare];
				
				// en passant captures
				if ((Piece::IsColor(piece, Piece::white) && startRank == 4) ||
					(Piece::IsColor(piece, Piece::black) && startRank == 3)) {
					
					if (targetFile == this->enPassantFile) 
						this->possibleMoves.push_back({ startSquare, targetSquare, Flag::EN_PASSANT });
				}
				
				// nothing to captures
				if (!pieceOnTargetSquare)
					continue;

				this->possibleMoves.push_back({ startSquare, targetSquare, Flag::MOVE });
			}
		}
	}

	// had to reset it and stuff yes
	this->enPassantFile = -1;
}

void Board::ClearPossibleMoveData() {
	this->possibleMoves.clear();
}

void Board::ClearHeldPieceMoveData() {
	this->heldPieceMoves.clear();
}

bool Board::IsMoveValid(int targetSquare) {
	return std::find(this->heldPieceMoves.begin(), this->heldPieceMoves.end(), targetSquare) != heldPieceMoves.end();		
}

bool Board::AreTwoSquaresInRange(int startSquare, int targetSquare, int range) {
	int deltaX = std::abs(this->GetIndexFile(startSquare) - this->GetIndexFile(targetSquare));
	int deltaY = std::abs(this->GetIndexRank(startSquare) - this->GetIndexRank(targetSquare));

	int max = std::max(deltaX, deltaY);

	return max == range;
}

int Board::GetMoveFlag(int startSquare, int targetSquare) {
	for (Move move : this->possibleMoves) {
		if (move.startSquare == startSquare && move.targetSquare == targetSquare) {
			return move.flag;
		}
	}
	// place holder in case shit happens :o
	return Flag::MOVE;
}

int Board::GetIndexRank(int index) {
	return index / 8;
}

int Board::GetIndexFile(int index) {
	return index - (this->GetIndexRank(index) * 8);
}
