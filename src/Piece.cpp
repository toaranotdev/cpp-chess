#include "Piece.hpp"

bool Piece::IsColor (int piece, int color) {
    return (piece & Piece::colorMask) == color;
}

int Piece::Color (int piece) {
    return piece & Piece::colorMask;
}

int Piece::Type (int piece) {
    return piece & Piece::typeMask;
}

bool Piece::IsRookOrQueen (int piece) {
	return (piece & 0b110) == 0b110;
}

bool Piece::IsBishopOrQueen (int piece) {
	return (piece & 0b101) == 0b101;
}

bool Piece::IsSlidingPiece (int piece) {
	return (piece & 0b100) != 0;
}