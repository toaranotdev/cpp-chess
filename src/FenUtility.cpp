#include "FenUtility.hpp"

FenUtility::FenUtility (int* Square) {
    this->Square = Square;
}

void FenUtility::LoadPosFromFen (std::string fenString) {
    
    // Empty our array to avoid segmentation fault
    for (int a = 0; a < 64; a ++) {
        this->Square[a] = 0;
    }


    std::string board = fenString.substr (0, fenString.find (" "));
    int file = 0; int rank = 7;

    for (char symbol : board) {
        if (symbol == '/') {
            file = 0; rank --;
        } else {
            if (isdigit (symbol)) {
                file += (int (symbol)) - 48; // Had to subtract because of ASCII stuff 
            } else {
                int pieceCol = isupper (symbol) ? this->piece.White : this->piece.Black;
                int pieceType = this->pieceTypeFromSymbol [tolower (symbol)];

                int index = rank * 8 + file;
                this->Square[index] = pieceCol | pieceType;
                file ++;
            }
        }
    }
}