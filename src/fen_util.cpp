#include "fen_util.h"

/*
 * also fuck those namespaces and their members
 */ 


// apparently you cannot use double quotes for chars otherwise shit happens
std::map <char, int> FenUtil::pieceMap {
	{'k', Piece::king}, 	{'q', Piece::queen},
	{'n', Piece::knight}, 	{'b', Piece::bishop},
	{'r', Piece::rook}, 	{'p', Piece::pawn}
};

/*
 * fuck those pointers and arrays
 */

void FenUtil::LoadFen(BoardData data, std::string fenString) {
	std::string fenBoard = fenString.substr(0, fenString.find(" "));
	char firstMover = (fenString.substr(fenString.find(" ") + 1, 1))[0];

	int file = 0;
	int rank = 7;

	for (char symbol : fenBoard) {
		if (symbol == '/') {
			file = 0;
			rank --;
		} else {
			if (isdigit(symbol)) {
				file += int(symbol) - 48; // had to subtract due to ASCII stuff
			} else {
				int color = (isupper(symbol)) ? Piece::white : Piece::black;
				int type = FenUtil::pieceMap[tolower(symbol)];
				
				int index = rank * 8 + file;
				data.squares[index] = color | type;
				
				file ++;
			}
		}
	}

	*(data.colorToMove) = (tolower(firstMover) == 'w') ? Piece::white : Piece::black;
}
