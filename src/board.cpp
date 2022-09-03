#include "board.h"

Board::Board() {
	FenUtil::LoadFen(this->squares, this->startingFen);
	
	this->CalculateNumSquaresToEdge();
}

void Board::CalculateNumSquaresToEdge() {
	for (int file = 0; file < 8; file ++) {
		for (int rank = 0; rank < 8; rank ++) {			
            		int numNorth = 7 - rank;
            		int numSouth = rank;
            		int numWest = file;
            		int numEast = 7 - file;

           		this->numSquaresToEdge.push_back({
                		numNorth,
                		numSouth,
                		numWest,
               			numEast,
               			std::min (numNorth, numWest),
                		std::min (numSouth, numEast),
                		std::min (numNorth, numEast),
                		std::min (numSouth, numWest)
			});
		}
	}
}

void Board::GeneratePossibleMoves() {
	for (int i = 0; i < 64; i ++) {
		int piece = Board::squares[i];

		if (piece) {
			switch (piece) {
				case Piece::bishop:
				case Piece::rook:
				case Piece::queen:
					this->GenerateSlidingPieceMoves(i, piece);
					break;
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
	

            		this->possibleMoves.push_back ({startSquare, targetSquare});

            		// target square contains enemy piece, can't move after capturing it
            		if (pieceOnTargetSquare != Piece::none) {
               			break;
            		}
		}
    	}
}
