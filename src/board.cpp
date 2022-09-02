#include "board.h"

Board::Board() {
	FenUtil::LoadFen(this->squares, this->startingFen);
}
