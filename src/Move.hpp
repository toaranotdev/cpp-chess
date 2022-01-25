#include "FenUtility.hpp"
#include <vector>

class Move {
    public: 

        int Max (int a, int b);
        int Min (int a, int b);

        int GetFileFromIndex (int index);
        int GetRankFromIndex (int index);

        void CalculateNumSquareToEdge();
        void GetMove (int startSquare);
        void GenerateMove();
        
        void GenerateKingMove();
        void GenerateKnightMove (int startSquare);
        void GeneratePawnMove();
        void GenerateSlidingMove (int startSquare, int piece);

        struct moveData {
            int startSquare;
            int targetSquare;
            int flag;
            /*
                0: Invalid
                1: Normal
                2: Double pawn push
                3: En passant
                4: Kingside castle
                5: Queenside castle
                6: Promotion
            */
        };

        bool IsValidMove (int targetSquare);
        void ClearData (int flag);

        int GetDeltaXFromSquare (int startSquare, int targetSquare);
        int GetDeltaYFromSquare (int startSquare, int targetSquare);

        std::vector <int> movesForHeldPiece;
        std::vector <std::vector <int>> numSquaresToEdge;
        
        int* Square;
        int* colorToMove;
    
    private:

        std::vector <moveData> possibleMoves;

        int directionOffsets[8] = {8, -8, -1, 1, 7, -7, 9, -9};
};