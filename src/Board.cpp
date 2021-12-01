#include "Board.hpp"

Board::Board() {
    this->InitializeSprite();
    this->InitializeBoard();
    this->heldPiece = 0;
    this->playerTurn = 0;
}

void Board::CreateGraphicalBoard (sf::RenderTarget* target, sf::Vector2i mousePos) {
    this->Square.setSize (sf::Vector2f (64.f, 64.f));
    for (int rank = 0; rank < 8; rank ++) {
        for (int file = 0; file < 8; file ++) {
            bool isLightSquare = (file + rank) % 2 == 0;

            this->Square.setPosition (sf::Vector2f (64.f * file, 64.f * rank));
            
            if (this->Square.getGlobalBounds().contains (sf::Vector2f (mousePos.x, mousePos.y))) {
                this->squarePointed = rank * 8 + file;
            }

            if (isLightSquare) {
                this->Square.setFillColor (sf::Color (140, 162, 173));
            } else {
                this->Square.setFillColor (sf::Color (222, 227, 230));
            }
            target->draw (this->Square);
        }
    }
}

void Board::InitializeSprite() {

    for (int i = 0; i < 64; i ++) {
        this->boardSquare[i] = 0;
    }

    this->texture.loadFromFile ("assets/piece.png");
    this->texture.setSmooth (true);
    
    // White pieces
    for (int i = 0; i < 6; i ++) {
        this->sprite[i].setTexture (this->texture);
        this->sprite[i].setTextureRect (sf::IntRect (2000 / 6 * i, 0, 2000 / 6, 334));
        this->sprite[i].setScale (sf::Vector2f (24.f / 125.f, 32.f / 167.f));
    }
    // Black pieces
    for (int i = 0; i < 6; i ++) {
        this->sprite[i + 6].setTexture (this->texture);
        this->sprite[i + 6].setTextureRect (sf::IntRect (2000 / 6 * i, 334, 2000 / 6, 334));
        this->sprite[i + 6].setScale (sf::Vector2f (24.f / 125.f, 32.f / 167.f));
    }
}

void Board::InitializeBoard() {
    this->boardSquare[0] = this->piece.Black | this->piece.Rook;
    this->boardSquare[1] = this->piece.Black | this->piece.Knight;
    this->boardSquare[2] = this->piece.Black | this->piece.Bishop;
    this->boardSquare[3] = this->piece.Black | this ->piece.Queen;
    this->boardSquare[4] = this->piece.Black | this->piece.King;
    this->boardSquare[5] = this->piece.Black | this->piece.Bishop;
    this->boardSquare[6] = this->piece.Black | this->piece.Knight;
    this->boardSquare[7] = this->piece.Black | this->piece.Rook;

    for (int i = 0; i < 8; i ++) {
        this->boardSquare[i + 8] = this->piece.Black | this->piece.Pawn;
    }
    
    this->boardSquare[56] = this->piece.White | this->piece.Rook;
    this->boardSquare[57] = this->piece.White | this->piece.Knight;
    this->boardSquare[58] = this->piece.White | this->piece.Bishop;
    this->boardSquare[59] = this->piece.White | this ->piece.Queen;
    this->boardSquare[60] = this->piece.White | this->piece.King;
    this->boardSquare[61] = this->piece.White | this->piece.Bishop;
    this->boardSquare[62] = this->piece.White | this->piece.Knight;
    this->boardSquare[63] = this->piece.White | this->piece.Rook;

    for (int i = 0; i < 8; i ++) {
        this->boardSquare[i + 48] = this->piece.White | this->piece.Pawn;
    }

}

void Board::DrawPiece (sf::RenderTarget* target, sf::Vector2i mousePos) {
    for (int rank = 0; rank < 8; rank ++) {
        for (int file = 0; file < 8; file ++) {
            int squareIndex = rank * 8 + file;
            int piece = this->boardSquare[squareIndex];
            if (piece != 0) {
                if (this->piece.Color (piece) == 8) {
                    sprite[this->piece.Type (piece ) - 1].setPosition (64.f * file, 64.f * rank);
                    target->draw (sprite[this->piece.Type (piece) - 1]);
                } else {
                    sprite[(this->piece.Type (piece)) + 5].setPosition (64.f * file, 64.f * rank);
                    target->draw (sprite[this->piece.Type (piece) + 5]);
                }
            }

            if (this->heldPiece != 0) {
                this->heldPieceType = this->piece.Type (this->heldPiece);
                this->heldPieceColor = this->piece.Color (this->heldPiece);
                if (this->piece.Color (this->heldPiece) == 8) {
                    sprite[this->heldPieceType - 1].setPosition (mousePos.x - 32, mousePos.y - 32);
                    target->draw (sprite[this->piece.Type (this->heldPiece) - 1]);
                } else {
                    sprite[this->heldPieceType + 5].setPosition (mousePos.x - 32, mousePos.y - 32);
                    target->draw (sprite[this->piece.Type (this->heldPiece) + 5]);
                }
            }
        }
    }
}

void Board::HandleInputOn(sf::Vector2i mousePos) {
    for (int i = 0; i < 12; i ++) {
        if (this->sprite[i].getGlobalBounds().contains (mousePos.x, mousePos.y) || this->boardSquare [this->squarePointed] != 0) {
            if (this->heldPiece == 0) {
                this->lastSquare = this->squarePointed;
                this->heldPiece = this->boardSquare[this->squarePointed];
                this->boardSquare[this->squarePointed] = 0;
            }
        }
    }
}

void Board::HandleInputOff () {
    this->boardSquare[this->squarePointed] = this->heldPiece;
    this->heldPiece = 0;
}
