#pragma once
#include <SFML/Graphics.hpp>
#include <iostream>
#include "Piece.hpp"

class Piece {
    public:
        const int None = 0;
        const int King = 1;
        const int Pawn = 2;
        const int Knight = 3;
        const int Bishop = 4;
        const int Rook = 5;
        const int Queen = 6;

        const int White = 8;
        const int Black = 16;

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
