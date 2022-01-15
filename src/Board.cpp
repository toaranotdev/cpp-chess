#include "Board.hpp"

Board::Board (sf::Vector2i* mousePos) : fenUtility (this->boardSquare) {

    // Initialize variables
    this->size = 64.f; // Size of the squares
    this->heldPiece = 0;

    // Initialize mouse
    this->mousePos = mousePos;

    this->InitializeSprite();
    this->InitializeBoard();

}
void Board::InitializeBoard() {
    std::string startFen = "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1";
    this->fenUtility.LoadPosFromFen (startFen);
}

void Board::InitializeSprite() {

    this->texture.loadFromFile ("assets/piece.png");
    this->texture.setSmooth (true);

    sf::Vector2f spriteScale = sf::Vector2f (this->size / (2000.f / 6.f), this->size / 334.f);
    
    // White pieces
    for (int i = 0; i < 6; i ++) {

        sf::IntRect textureRect = sf::IntRect (2000.f / 6.f * i, 0, 2000.f / 6.f, 334.f);

        this->sprite[i].setTexture (this->texture);
        this->sprite[i].setTextureRect (textureRect);
        this->sprite[i].setScale (spriteScale);
    }
    // Black pieces
    for (int i = 0; i < 6; i ++) {

        sf::IntRect textureRect = sf::IntRect (2000 / 6 * i, 334, 2000 / 6, 334);

        this->sprite[i + 6].setTexture (this->texture);
        this->sprite[i + 6].setTextureRect (textureRect);
        this->sprite[i + 6].setScale (spriteScale);
    }
}

void Board::CreateGraphicalBoard (sf::RenderTarget* target) {
    this->Square.setSize (sf::Vector2f (this->size, this->size));

    sf::Color darkCol = sf::Color (181, 136, 99);
    sf::Color lightCol = sf::Color (240, 217, 181);

    for (int rank = 0; rank < 8; rank ++) {
        for (int file = 0; file < 8; file ++) {

            bool isLightSquare = (file + rank) % 2 == 0;

            sf::Color squareCol = (isLightSquare) ? lightCol : darkCol;

            this->Square.setFillColor (squareCol);
            this->Square.setPosition (sf::Vector2f (this->size * file, this->size * rank));

            target->draw (this->Square);
        }
    }
}

void Board::DrawPiece (sf::RenderTarget* target) {
    for (int rank = 0; rank < 8; rank ++) {
        for (int file = 0; file < 8; file ++) {

            // To make the index starts at the top left square, 7 - rank does that
            int squareIndex = rank * 8 + file;
            int piece = this->boardSquare[squareIndex];
            
            // Skip drawing if square contains no pieces
            if (piece == this->piece.None) {
                continue;
            }

            int pieceType = this->piece.Type (piece);
            int pieceCol = this->piece.Color (piece);

            // Due to how the sprites are set up +5 or -1 is needed to be add to the index
            int offset = (pieceCol == this->piece.White) ? 5 : -1;
            int index = this->spriteIndexFromType[pieceType] + offset;
            
            sprite[index].setPosition (sf::Vector2f (this->size * file, this->size * rank));
            target->draw (sprite[index]);

        }
    }

    if (this->heldPiece != 0) {
        
        this->heldPieceType = this->piece.Type (this->heldPiece);
        this->heldPieceCol = this->piece.Color (this->heldPiece);

        int offset = (this->heldPieceCol == this->piece.White) ? 5 : -1;
        int index = this->spriteIndexFromType[this->heldPieceType] + offset;

        sf::Vector2f position = sf::Vector2f (this->mousePos->x - (this->size / 2.f), this->mousePos->y - (this->size / 2.f));

        sprite[index].setPosition (position);
        target->draw (sprite[index]);

    }
}

void Board::HandleInputOn() {
    int squareUnderMouse = this->GetSquareUnderMouse ();
    int pieceOnSquareUnderMouse = this->boardSquare[squareUnderMouse];

    if (pieceOnSquareUnderMouse != this->piece.None) {
        if (this->heldPiece == this->piece.None) {

            this->lastSquareClicked = squareUnderMouse;
            this->heldPiece = pieceOnSquareUnderMouse;
            this->boardSquare[this->lastSquareClicked] = 0;
        
        }
    }
}

void Board::HandleInputOff () {
    // To avoid placing an empty piece, this will change when legal moves are added
    if (this->heldPiece != 0) {
        int squareUnderMouse = this->GetSquareUnderMouse();
        this->boardSquare[squareUnderMouse] = this->heldPiece;
        this->heldPiece = this->piece.None;
    }
}

int Board::GetSquareUnderMouse () {
    int rank = this->mousePos->y / this->size;
    int file = this->mousePos->x / this->size;
    int index = rank * 8 + file;

    return index;
}