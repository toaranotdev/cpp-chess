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
				case Piece::queen: 

				{
					this->GenerateSlidingPieceMoves(i, piece);
					break;
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

            		this->possibleMoves.push_back ({ startSquare, targetSquare });

			// target square contains enemy piece, can't move further after capturing it
            		if (pieceOnTargetSquare != Piece::none) {
               			break;
            		}
		}
    	}
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

int Board::GetIndexRank(int index) {
	return index / 8;
}

int Board::GetIndexFile(int index) {
	return index - (this->GetIndexRank(index) * 8);
}
