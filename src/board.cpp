#include "board.h"

bool Move::operator==(const Move& move) const {
	return startSquare == move.startSquare && targetSquare == move.targetSquare && flag == move.flag;
}

Board::Board() {
	BoardData data = this->ExportData();
	FenUtil::LoadFen(data, this->startingFen);
	
	this->CalculateNumSquaresToEdge();
	this->CalculateMoves();
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
void Board::CalculateMoves() {
	this->ClearMoveData();

	for (int i = 0; i < 64; i ++) {
		int piece = Board::squares[i];

		int color = Piece::GetColor(piece);
		int type = Piece::GetType(piece);

		std::list<Move>* movesList = &((this->colorToMove == color) ? this->possibleMoves : this->opponentsPossibleMoves);

		if (piece) {
			switch (type) {
				case Piece::bishop:
				case Piece::rook:
				case Piece::queen: {
					std::vector<Move> slidingPieceMoves = this->GenerateSlidingPieceMoves(i, piece);
					movesList->insert(movesList->end(), slidingPieceMoves.begin(), slidingPieceMoves.end());
					break;
				}
				case Piece::knight: {
					std::vector<Move> knightMoves = this->GenerateKnightMoves(i);
					movesList->insert(movesList->end(), knightMoves.begin(), knightMoves.end());
					break;
				}
				case Piece::king: {
					this->friendlyKingSquare = (this->colorToMove == color) ? i : this->friendlyKingSquare;

					std::vector<Move> kingMoves = this->GenerateKingMoves(i, color);
					movesList->insert(movesList->end(), kingMoves.begin(), kingMoves.end());
					break;
				}
				case Piece::pawn: {
					std::vector<Move> pawnMoves = this->GeneratePawnMoves(i, color);
					movesList->insert(movesList->end(), pawnMoves.begin(), pawnMoves.end());
				}
				default:
					break;
			}
		}
	}
	// clear en passant data because asjdisadjsaidsajdaiodjsadpasd
	this->enPassantFile = -1;
	this->FilterMove();
}

std::vector<Move> Board::GenerateSlidingPieceMoves (int startSquare, int piece) {
	std::vector<Move> moves;
	
	int startDirIndex = (Piece::IsType (piece, Piece::bishop)) ? 4 : 0;
    	int endDirIndex = (Piece::IsType (piece, Piece::rook)) ? 4 : 8;


    	for (int directionIndex = startDirIndex; directionIndex < endDirIndex; directionIndex ++) {
		for (int square = 0; square < this->numSquaresToEdge[startSquare][directionIndex]; square ++) {
			
			int targetSquare = startSquare + this->directionOffsets[directionIndex] * (square + 1);
            		moves.push_back ({ startSquare, targetSquare, Flag::MOVE });
		}
    	}

	return moves;
}

std::vector<Move> Board::GenerateKnightMoves(int startSquare) {
	std::vector<Move> moves;

	const int knightJumps[] = { 15, 17, -15, -17, 6, 10, -6, -10 };

	for (int offset : knightJumps) {
		int targetSquare = startSquare + offset;
			
		// make sure the knights only move two squares
		// on the x, y axis to eliminate indecies that wrapped around the board
		bool isRangeValid = (this->AreTwoSquaresInRange(startSquare, targetSquare, 2) &&
					targetSquare < 64 && targetSquare >= 0);
	
		if (isRangeValid)
			moves.push_back({ startSquare, targetSquare, Flag::MOVE });
	}

	return moves;
}

std::vector<Move> Board::GenerateKingMoves (int startSquare, int color) {
	std::vector<Move> moves;

	const int kingMoves[] = { 1, -1, 8, -8, 7, -7, 9, -9 };

	for (int o : kingMoves) {
		int targetSquare = startSquare + o;
		bool isRangeValid = (this->AreTwoSquaresInRange(startSquare, targetSquare, 1)) && targetSquare >= 0 && targetSquare < 64;
		
		if (isRangeValid) 
			moves.push_back({ startSquare, targetSquare, Flag::MOVE });
	}

	bool isWhitePiece = (color == Piece::white);

	int kingSideRookIndex = (isWhitePiece) ? 7 : 63;
	int queenSideRookIndex = (isWhitePiece) ? 0 : 56;
	
	int kingSideKingIndex = (isWhitePiece) ? 6 : 62;
	int queenSideKingIndex = (isWhitePiece) ? 2 : 58;

	int* castleRights = &((isWhitePiece) ? this->whiteCastleRights : this->blackCastleRights);
	
	bool canCastleKingSide = (*castleRights & this->kingSideCastleMask) == this->kingSideCastleMask;
	bool canCastleQueenSide = (*castleRights & this->queenSideCastleMask) == this->queenSideCastleMask;

	bool hasKingMoved = (startSquare != ((isWhitePiece) ? 4 : 60));

	for (int i = 0; i < 2; i ++) {
		bool isCastleKingSide = (i == 0);
		bool canCastle = (isCastleKingSide) ? canCastleKingSide : canCastleQueenSide;

		int targetSquare = (isCastleKingSide) ? kingSideKingIndex : queenSideKingIndex;
		int rookIndex = (isCastleKingSide) ? kingSideRookIndex : queenSideRookIndex;
		int range = std::abs(startSquare - targetSquare);
		
		for (int i = 1; i <= range; i ++) {
			int index = (isCastleKingSide) ? startSquare + i : startSquare - i;
			int piece = this->squares[index];
			// something is in the way, disable castling for that side for now
			if (piece) {
				goto endLoop;
			}
		}

		if (canCastle) {
			bool isRookPresent = (Piece::IsType(this->squares[rookIndex], Piece::rook));
		
			int flag = (isCastleKingSide) ? Flag::CASTLE_KING_SIDE : Flag::CASTLE_QUEEN_SIDE;
			
			// rook isn't present or king has moved
			// disable castling for that side entirely
			if (!isRookPresent || hasKingMoved) {
					*castleRights = (isCastleKingSide) ? 0b0011 & *castleRights : 0b1100 & *castleRights;	
					continue;
			}

			moves.push_back({ startSquare, targetSquare, flag });
		}

		endLoop:;
	}

	return moves;
}

std::vector<Move> Board::GeneratePawnMoves (int startSquare, int color) {
	std::vector<Move> moves;

	const int pawnMoves[] = { 8, 7, 9 };

	bool isWhitePiece = (color == Piece::white);
	
	int directionOffset = (color == Piece::white) ? 1 : -1;
	int startRank = this->GetIndexRank(startSquare);

	for (int offset : pawnMoves) {
		int targetSquare = startSquare + offset * directionOffset;
		int targetRank = this->GetIndexRank(targetSquare);
		
		// pushes
		if (offset == 8) {
			// make sure the pawn only moves 1 square blablabla
			bool isValid = this->AreTwoSquaresInRange(startSquare, targetSquare, 1) && targetSquare >= 0 && targetSquare <= 63;
			if (isValid) {
				int pieceOnTargetSquare = this->squares[targetSquare];
				// something is blocking the way, we're not gonna look any
				// further
				if (pieceOnTargetSquare)
					continue;

				int flag = (targetRank == 7 || targetRank == 0) ? Flag::PROMOTION : Flag::MOVE;

				moves.push_back({ startSquare, targetSquare, flag });
				
				// double pawn pushes
				if ((isWhitePiece && startRank == 1) || (!isWhitePiece && startRank == 6)) {

					targetSquare = startSquare + offset * directionOffset * 2;
					pieceOnTargetSquare = this->squares[targetSquare];

					if (pieceOnTargetSquare)
						continue;

					moves.push_back({ startSquare, targetSquare, Flag::DOUBLE_PUSH });
				}
			}
		// captures
		} else if (offset == 7 || offset == 9) {
			int targetSquare = startSquare + offset * directionOffset;
			bool isValid = this->AreTwoSquaresInRange(startSquare, targetSquare, 1);
			
			if (isValid) {
				int targetFile = this->GetIndexFile(targetSquare);
				int pieceOnTargetSquare = this->squares[targetSquare];
				int flag = (targetRank == 7 || targetRank == 0) ? Flag::PROMOTION : Flag::MOVE;
				
				// en passant captures
				if ((isWhitePiece && startRank == 4) || (!isWhitePiece && startRank == 3)) {

					if (targetFile == this->enPassantFile)
						moves.push_back({ startSquare, targetSquare, Flag::EN_PASSANT });
				}
								
				// nothing to captures or the piece to capture is of the same color
				if (!pieceOnTargetSquare || Piece::IsColor(pieceOnTargetSquare, color))
					continue;

				moves.push_back({ startSquare, targetSquare, flag });
			}
		}
	}
	return moves;
}

void Board::ClearMoveData() {
	this->possibleMoves.clear();
	this->opponentsPossibleMoves.clear();
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

bool Board::AreTwoSquaresOnSameLine(int startSquare, int targetSquare, int directionOffset) {
	switch (directionOffset) {
		case 1:
		case -1: {
			int startRank = this->GetIndexRank(startSquare);
			int targetRank = this->GetIndexRank(targetSquare);
			return (startRank == targetRank);
		}
		case 8:
		case -8: {
			int startFile = this->GetIndexFile(startSquare);
			int targetFile = this->GetIndexFile(targetSquare);
			return (startFile == targetFile);
		}
		case 7:
		case -7:
		case 9:
		case -9: {
			return (targetSquare - startSquare) % directionOffset == 0;
		}
		default:
			 break;
	}
	return false;
}

int Board::GetDirectionOffset(int startSquare, int targetSquare) {
	const int directionOffsets[] = { 1, 8, 7, 9 };
	int offset = (startSquare < targetSquare) ? 1 : -1;

	for (int directionOffset : directionOffsets) {
		switch (directionOffset) {
			case 1: {
				int startRank = this->GetIndexRank(startSquare);
				int targetRank = this->GetIndexRank(targetSquare);
				if (startRank == targetRank)
					return offset * directionOffset;
				break;
			}
			case 8: {
				int startFile = this->GetIndexFile(startSquare);
				int targetFile = this->GetIndexFile(targetSquare);
				if (startFile == targetFile)
					return offset * directionOffset;
				break;
			}
			case 7:
			case 9: {
				if ((startSquare - targetSquare) % directionOffset == 0)
					return offset * directionOffset;
				break;
			}
			default:
				break;
		}	
	}
	return 0;
}

bool Board::IsSquareUnderEnemyControl(int startSquare, int targetSquare) {
	auto f = [&startSquare, &targetSquare](Move &move) { return move.targetSquare == targetSquare; };
	return std::find_if(this->opponentsPossibleMoves.begin(), this->opponentsPossibleMoves.end(), f) != this->opponentsPossibleMoves.end();
}

void Board::FilterMove() {
	std::list<Move>::iterator i = this->possibleMoves.begin();

	while (i != this->possibleMoves.end()) {
		Move move = *(i);

		int pieceOnStartSquare = this->squares[move.startSquare];
		int pieceOnTargetSquare = this->squares[move.targetSquare];

		int pieceOnStartSquareType = Piece::GetType(pieceOnStartSquare);

		switch (pieceOnStartSquareType) {
			case Piece::bishop:
			case Piece::rook:
			case Piece::queen: {
				if (pieceOnTargetSquare) {
					int directionOffset = this->GetDirectionOffset(move.startSquare, move.targetSquare);
					int directionIndex = this->directionIndexMap[directionOffset];
					
					int numSquaresToEdge = (this->numSquaresToEdge[move.targetSquare][directionIndex]);

					std::list<Move>::iterator beginIterator = (Piece::IsColor(pieceOnTargetSquare, this->colorToMove)) ? i : std::next(i, 1);
					std::list<Move>::iterator endIterator = std::next(i, numSquaresToEdge + 1);

					this->possibleMoves.erase(beginIterator, endIterator);

					i = std::move(endIterator);
					continue;
				}
				break;
			}
			case Piece::knight:
			case Piece::king: {
				if  (pieceOnTargetSquare && Piece::IsColor(pieceOnTargetSquare, this->colorToMove)) {
					i ++;
					this->possibleMoves.remove(move);
					continue;
				}
				break;
			}
			default:
				break;
		}

		i ++;
	}
}

int Board::GetMoveFlag(int startSquare, int targetSquare) {
	auto f = [startSquare, targetSquare](Move move) { return move.startSquare == startSquare && move.targetSquare == targetSquare; };
	std::list<Move>::iterator i = std::find_if(this->possibleMoves.begin(), this->possibleMoves.end(), f);
	return (i != this->possibleMoves.end()) ? i->flag : Flag::MOVE;
}

int Board::GetIndexRank(int index) {
	return index / 8;
}

int Board::GetIndexFile(int index) {
	return index - (this->GetIndexRank(index) * 8);
}
