#include "Piece.hpp"

int Piece::Color (int piece) {
    return piece & Piece::colorMask;
}

int Piece::Type (int piece) {
    return piece & Piece::typeMask;
}

bool Piece::IsColor (int piece, int color) {
    return (piece & Piece::colorMask) == color;
}

bool Piece::IsType (int piece, int type) {
    return (piece & Piece::typeMask) == type;
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