#pragma once
#include <SFML/Graphics.hpp>
#include <iostream>
#include "Piece.hpp"

class Piece {
    public:
        const static int None = 0;
        const static int King = 1;
        const static int Pawn = 2;
        const static int Knight = 3;
        const static int Bishop = 4;
        const static int Rook = 5;
        const static int Queen = 6;

        const static int White = 8;
        const static int Black = 16;

        const static int typeMask = 0b00111;
        const static int whiteMask = 0b10000;
        const static int blackMask = 0b01000;
        const static int colorMask = whiteMask | blackMask;

        static int Color (int piece);
        static int Type (int piece);
        
        static bool IsColor (int piece, int color);
        static bool IsType (int piece, int type);

        static bool IsSlidingPiece (int piece);
        static bool IsRookOrQueen (int piece);
        static bool IsBishopOrQueen (int piece);

};
