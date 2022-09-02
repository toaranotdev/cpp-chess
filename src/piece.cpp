#include "piece.h"

bool Piece::IsType(int piece, int type) {
	return (piece & Piece::typeMask) == type;
}

bool Piece::IsColor(int piece, int color) {
	return (piece & Piece::colorMask) == color;
}

int Piece::GetType(int piece) {
	return (piece & Piece::typeMask);
}

int Piece::GetColor(int piece) {
	return (piece & Piece::colorMask);
}
