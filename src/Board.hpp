#pragma once
#include "Move.hpp"
#include <SFML/Audio.hpp>

class Board {
    public:
        Board (sf::Vector2i* mousePos);

        void CreateGraphicalBoard (sf::RenderTarget* target);
        void DrawPiece (sf::RenderTarget* target);
        void DrawHighlight (sf::RenderTarget* target);

        void HandleInputOn ();
        void HandleInputOff ();

        int GetSquareUnderMouse();
        
        void InitializeSprite();
        void InitializeBoard();
        // Instead of setting up things using constructors, i'd like to control when to call it
        void InitializeMove();

        int boardSquare[64];
        int colorToMove;
        
        int heldPiece;
        int heldPieceType;
        int heldPieceCol;

    private:

        void PlaySound (int flag);

        sf::RectangleShape Square;
        sf::RectangleShape Highlight;

        sf::Color highlightColor;
        
        sf::Texture texture;
        sf::Sprite sprite[12];

        sf::SoundBuffer Buffer;
        sf::Sound Sound;

        // Size of one tile
        float size;
        
        int lastSquareClicked;
        
        sf::Vector2i* mousePos;
        Move move;
        
        FenUtility fenUtility;

        std::map <int, int> spriteIndexFromType = {
            {Piece::King, 1}  , {Piece::Queen, 2},
            {Piece::Bishop, 3}, {Piece::Knight, 4},
            {Piece::Rook, 5}  , {Piece::Pawn, 6}
        };
};