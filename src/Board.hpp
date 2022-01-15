#pragma once
#include "FenUtility.hpp"


class Board {
    public:
        Board (sf::Vector2i* mousePos);

        void CreateGraphicalBoard (sf::RenderTarget* target);
        void DrawPiece (sf::RenderTarget* target);

        void HandleInputOn ();
        void HandleInputOff ();

        int GetSquareUnderMouse();
        
        void InitializeSprite();
        void InitializeBoard();

    private:

        sf::RectangleShape Square;
        sf::Texture texture;
        sf::Sprite sprite[12];

        Piece piece;

        int heldPiece;
        int heldPieceType;
        int heldPieceCol;

        float size;
        
        int lastSquareClicked;

        int boardSquare[64];
        
        sf::Vector2i* mousePos;
        FenUtility fenUtility;

        std::map <int, int> spriteIndexFromType = {
            {this->piece.King, 1}, {this->piece.Queen, 2},
            {this->piece.Bishop, 3}, {this->piece.Knight, 4},
            {this->piece.Rook, 5}, {this->piece.Pawn, 6}
        };
};