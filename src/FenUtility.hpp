#pragma once
#include <iostream>
#include <map>
#include <vector>
#include "Piece.hpp"

class FenUtility {
    public:

        FenUtility (int* Square);

        void LoadPosFromFen (std::string fenString);

    private:

        Piece piece;

        std::map <char, int> pieceTypeFromSymbol {
            {'k', piece.King}  , {'q', piece.Queen},
            {'n', piece.Knight}, {'b', piece.Bishop},
            {'r', piece.Rook}  , {'p', piece.Pawn}
        };

        int* Square;

};