#pragma once
#include <SFML/Graphics.hpp>
#include <iostream>
#include "Piece.hpp"

class Piece {
    public:
        // Variables
        const int None = 0;
        const int King = 1;
        const int Queen = 2;
        const int Bishop = 3;
        const int Knight = 4;
        const int Rook = 5;
        const int Pawn = 6;

        const int White = 8;
        const int Black = 16;

        // 0b: Indicating these bytes are for an integer
        // 0 : The bytes that we don't care about
        // 1 : The bytes that we actually need

        const int typeMask = 0b00111;
        const int whiteMask = 0b10000;
        const int blackMask = 0b01000;
        const int colorMask = whiteMask | blackMask;

        int Color (int piece);
        int Type (int piece);
        bool IsColor (int piece, int color);
        bool IsSlidingPiece (int piece);
        bool IsRookOrQueen (int piece);
        bool IsBishopOrQueen (int piece);

};
