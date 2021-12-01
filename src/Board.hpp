#pragma once
#include "Piece.hpp"


class Board {
    public:
        Board();

        void CreateGraphicalBoard (sf::RenderTarget* target, sf::Vector2i mousePos);
        void DrawPiece (sf::RenderTarget* target, sf::Vector2i mousePos);
        void HandleInputOn (sf::Vector2i mousePos);
        void Update();
        void HandleInputOff ();
        void InitializeSprite();
        void InitializeBoard();

    private:

        sf::RectangleShape Square;
        sf::Texture texture;
        sf::Sprite sprite[12];

        Piece piece;

        int heldPiece;
        int heldPieceType;
        int heldPieceColor;
        
        int squarePointed;
        int lastSquare;

        int playerTurn;

        int boardSquare[64];

};