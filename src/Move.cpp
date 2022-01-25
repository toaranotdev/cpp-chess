#include "Move.hpp"

int Move::Max (int a, int b) {
    return (a > b) ? a : b;
}

int Move::Min (int a, int b) {
    return (a < b) ? a : b;
}

int Move::GetFileFromIndex (int index) {
    return index - ((index / 8) * 8);
}

int Move::GetRankFromIndex (int index) {
    return 7 - (index / 8);
}

void Move::CalculateNumSquareToEdge() {

    for (int rank = 0; rank < 8; rank ++) {
        for (int file = 0; file < 8; file ++) {

            int numNorth = 7 - rank;
            int numSouth = rank;
            int numWest = file;
            int numEast = 7 - file;

            this->numSquaresToEdge.push_back ({
                numNorth,
                numSouth,
                numWest,
                numEast,
                this->Min (numNorth, numWest),
                this->Min (numSouth, numEast),
                this->Min (numNorth, numEast),
                this->Min (numSouth, numWest)
            });

        }
    }
}

// Generate all the possible moves
void Move::GenerateMove () {

    int colorToMove = * (this->colorToMove);

    for (int startSquare = 0; startSquare < 64; startSquare ++) {
        
        int piece = this->Square[startSquare];

        if (Piece::IsColor (piece, colorToMove) && piece != Piece::None) {
            if (Piece::IsSlidingPiece (piece)) {
                this->GenerateSlidingMove (startSquare, piece);
            } else if (Piece::Type (piece) == Piece::Knight) {
                this->GenerateKnightMove (startSquare);
            }
        }
    }
}

// Get targetSquares from startSquare
void Move::GetMove (int startSquare) {

    for (auto move = this->possibleMoves.begin(); move != this->possibleMoves.end(); move ++) {
        if (move->startSquare == startSquare) {
            this->movesForHeldPiece.push_back (move->targetSquare);
        }
    }
}

// Bishop, Queen, Rook movement
void Move::GenerateSlidingMove (int startSquare, int piece) {

    int startDirIndex = (Piece::IsType (piece, Piece::Bishop)) ? 4 : 0;
    int endDirIndex = (Piece::IsType (piece, Piece::Rook)) ? 4 : 8;

    for (int directionIndex = startDirIndex; directionIndex < endDirIndex; directionIndex ++) {
        for (int square = 0; square < this->numSquaresToEdge[startSquare][directionIndex]; square ++) {

            int targetSquare = startSquare + directionOffsets[directionIndex] * (square + 1);
            int pieceOnTargetSquare = this->Square[targetSquare];

            // Blocked by piece of the same color, can't move
            if (Piece::IsColor (pieceOnTargetSquare, *(colorToMove))) {
                if (pieceOnTargetSquare != Piece::None) {
                    break;
                }
            }

            this->possibleMoves.push_back ({startSquare, targetSquare, 1});

            // Target square contains enemy piece, can't move after capturing it
            if (pieceOnTargetSquare != Piece::None) {
                break;
            }

        }
    }
}

void Move::GenerateKnightMove (int startSquare) {
    int knightJumps[8] = {10, -10, 6, -6, 17, -17, 15, -15};

    for (int a : knightJumps) {
        int targetSquare = startSquare + a;
        int pieceOnTargetSquare = this->Square[targetSquare];

        // Skip if the piece on target square is our teammate
        if (Piece::IsColor (pieceOnTargetSquare, * (this->colorToMove))) {
            continue;
        }
        
        int knightJumpDeltaX = this->GetDeltaXFromSquare (startSquare, targetSquare);
        int knightJUmpDeltaY = this->GetDeltaYFromSquare (startSquare, targetSquare);

        int moveDst = this->Max (knightJumpDeltaX, knightJUmpDeltaY);

        // Ensure knight only moves 2 squares on the x and y axis
        if (moveDst == 2) {
            this->possibleMoves.push_back ({startSquare, targetSquare, 1});
        } 
    }
}

bool Move::IsValidMove (int targetSquare) {

    // Target square wasn't found in our list of possible moves for that start square
    if (std::find (this->movesForHeldPiece.begin(), this->movesForHeldPiece.end(), targetSquare) == this->movesForHeldPiece.end()) {
        return false;
    }

    return true;
}
// Clear everything like possible moves
void Move::ClearData (int flag) {
    /*
    Flags: 
        1: Clear possible moves array
        2: Clear moves for held piece
        3: Clear everything
    */
   switch (flag) {
        case 1:
            this->possibleMoves.clear();
            break;
        case 2:
            this->movesForHeldPiece.clear();
            break;
        case 3:
            this->possibleMoves.clear();
            this->movesForHeldPiece.clear();
            break;
   };
}

int Move::GetDeltaXFromSquare (int startSquare, int targetSquare) {
    int startX = this->GetRankFromIndex (startSquare);
    int targetX = this->GetRankFromIndex (targetSquare);

    return abs (targetX - startX);
}

int Move::GetDeltaYFromSquare (int startSquare, int targetSquare) {
    int startY = this->GetFileFromIndex (startSquare);
    int targetY = this->GetFileFromIndex (targetSquare);

    return abs (targetY - startY);
}