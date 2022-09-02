#pragma once

namespace Piece {
	const int none		= 0; // this value should always be 0

	const int king 		= 1; // 0b00001
	
	const int bishop 	= 2; // 0b00010
	const int knight 	= 3; // 0b00011
	const int pawn 		= 4; // 0b00100
	const int rook 		= 5; // 0b00101
	const int queen 	= 6; // 0b00110
	
	const int white		= 8;
	const int black		= 16;

	const int colorMask 	= 0b11000;
	const int typeMask 	= 0b00111;
	
	bool IsType(int piece, int type);
	bool IsColor(int piece, int color);

	int GetType(int piece);
	int GetColor(int color);
};
