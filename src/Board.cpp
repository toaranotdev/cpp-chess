#include "Board.hpp"

Board::Board (sf::Vector2i* mousePos) : fenUtility (this->boardSquare) {

    // Initialize variables
    this->size = 85.f;
    this->heldPiece = Piece::None;
    this->colorToMove = Piece::White;

    // Initialize mouse
    this->mousePos = mousePos;

    this->InitializeSprite();
    this->InitializeBoard();
    this->InitializeMove();

}

void Board::InitializeMove () {
    this->move.Square = this->boardSquare;
    this->move.colorToMove = & (this->colorToMove);

    this->move.CalculateNumSquareToEdge();
    this->move.GenerateMove();
}


void Board::InitializeBoard () {
    // Initalize board square sprite
    sf::Vector2f size = sf::Vector2f (this->size, this->size);
    this->Square.setSize (size);

    // Initialize highlight square
    this->highlightColor = sf::Color (255, 99, 107, 99);
    this->Highlight.setSize (size);
    this->Highlight.setFillColor (this->highlightColor);

    std::string startFen = "nrqb/8/8/8/8/8/8/RBQ w KQkq - 0 1";
    this->fenUtility.LoadPosFromFen (startFen);
}


void Board::InitializeSprite() {

    this->texture.loadFromFile ("assets/sprites/pieces.png");
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

    sf::Color darkCol = sf::Color (181, 136, 99);
    sf::Color lightCol = sf::Color (240, 217, 181);

    for (int file = 0; file < 8; file ++) {
        for (int rank = 0; rank < 8; rank ++) {

            bool isLightSquare = (file + rank) % 2 == 0;

            sf::Color squareCol = (isLightSquare) ? lightCol : darkCol;

            this->Square.setFillColor (squareCol);
            this->Square.setPosition (sf::Vector2f (this->size * file, this->size * rank));

            target->draw (this->Square);
        }
    }
}

void Board::DrawPiece (sf::RenderTarget* target) {
    for (int rank = 7; rank >= 0; rank --) {
        for (int file = 0; file < 8; file ++) {

            int squareIndex = rank * 8 + file;
            int piece = this->boardSquare[squareIndex];
            
            // Skip if square contained no pieces
            if (piece == Piece::None) {
                continue;
            }

            int pieceType = Piece::Type (piece);

            int offset = (Piece::IsColor (piece, Piece::White)) ? -1 : 5;
            int index = this->spriteIndexFromType[pieceType] + offset;

            sf::Vector2f pos = sf::Vector2f (this->size * file, this->size * (7 - rank));
            
            sprite[index].setPosition (pos);
            target->draw (sprite[index]);

        }
    }

    if (this->heldPiece != Piece::None) {
        
        this->heldPieceType = Piece::Type (this->heldPiece);
        this->heldPieceCol = Piece::Color (this->heldPiece);

        int offset = (Piece::IsColor (this->heldPiece, Piece::White)) ? -1 : 5;
        int index = this->spriteIndexFromType[this->heldPieceType] + offset;

        sf::Vector2f position = sf::Vector2f (this->mousePos->x - (this->size / 2.f), this->mousePos->y - (this->size / 2.f));
        sprite[index].setPosition (position);
        target->draw (sprite[index]);

    }
}

void Board::HandleInputOn() {
    int squareUnderMouse = this->GetSquareUnderMouse ();
    int pieceOnSquareUnderMouse = this->boardSquare[squareUnderMouse];

    if (pieceOnSquareUnderMouse != Piece::None) {
        if (this->heldPiece == Piece::None) {

            this->lastSquareClicked = squareUnderMouse;
            this->move.GetMove (this->lastSquareClicked);
            this->heldPiece = pieceOnSquareUnderMouse;
            this->boardSquare[this->lastSquareClicked] = Piece::None;
        
        }
    }
}

void Board::HandleInputOff () {
    int squareUnderMouse = this->GetSquareUnderMouse();
    int flag = 0; // Audio flag

    if (this->heldPiece == 0) {
        return;
    }

    if (this->move.IsValidMove (squareUnderMouse)) {
        
        int offset = (this->boardSquare[squareUnderMouse] != Piece::None) ? 2 : 1;

        this->boardSquare[squareUnderMouse] = this->heldPiece;
        this->heldPiece = Piece::None;

        int nextColor = (this->colorToMove == Piece::White) ? Piece::Black : Piece::White;
        this->colorToMove = nextColor;

        this->move.ClearData (3);
        this->move.GenerateMove();

        flag += offset;

    } else {
        this->boardSquare[this->lastSquareClicked] = this->heldPiece;
        this->heldPiece = 0;
        this->move.ClearData (2);
        
        int offset = (squareUnderMouse == this->lastSquareClicked) ? 0 : 3;
        flag += offset;
    }

    if (flag != 0) {
        this->PlaySound (flag);
    }
}

int Board::GetSquareUnderMouse () {
    int rank = this->mousePos->y / this->size;
    int file = this->mousePos->x / this->size;
    int index = (7 - rank) * 8 + file;
    
    // Out of accepted values ? Return -1 (invalid) if yes;
    return (index < 0 || index > 63) ? -1 : index;
}

void Board::DrawHighlight (sf::RenderTarget* target) {
    if (this->move.movesForHeldPiece.empty()) {
        return;
    } else {
        for (int a : this->move.movesForHeldPiece) {

            int file = this->move.GetFileFromIndex (a);
            int rank = this->move.GetRankFromIndex (a);

            this->Highlight.setPosition (this->size * file, this->size * rank);
            target->draw (this->Highlight);
        }
    }
}

void Board::PlaySound (int flag) {
    /*
    1: Move
    2: Capture
    3: Error
    */
    switch (flag) {
        case 1:
            this->Buffer.loadFromFile ("assets/sfx/Move.ogg");
            break;
        case 2:
            this->Buffer.loadFromFile ("assets/sfx/Capture.ogg");
            break;
        case 3:
            this->Buffer.loadFromFile ("assets/sfx/Error.ogg");
            break;
        default:
            break;
    };

    this->Sound.setBuffer (this->Buffer);
    this->Sound.play();

}