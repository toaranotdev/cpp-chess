#include "game.h"
#include "piece.h"

Game::Game() {
	this->InitializeMembers();
}

void Game::InitializeMembers() {
	this->InitializeWindow();
	this->InitializeSprites();

}

void Game::InitializeWindow() {
	std::string title = "Chess"; // window title
	sf::VideoMode mode = sf::VideoMode(680, 680); // window size, etc...

	this->window = std::unique_ptr<sf::RenderWindow>(new sf::RenderWindow(mode, title));
}

void Game::InitializeSprites() {
	this->InitializeTileSprite();
	this->InitializePieceSprite();
}

void Game::InitializePieceSprite() {
	if (!this->texture.loadFromFile("./assets/textures/pieces.png")) {
		this->window->close();
	} else {
		this->texture.setSmooth(false);
		
		float size = 334; // size of one sprite in that whole sprite atlas
		sf::Vector2f scaleFactor (this->tileSize.x / size, this->tileSize.y / size);
		
		for (int i = 0; i < 12; i ++) {
			// pointers woo, we don't have to delete this, since 
			// we did not ask for memory (i.e: using new keyword)
			sf::Sprite* sprite = &(this->sprites[i]);

			// epic math to figure out where to cut the texture and shit
			int shiftX = (i > 6) ? (i - 6) * size : i * size;
			int shiftY = (i > 6) ?  size : 0;

			sf::IntRect spriteRect (shiftX, shiftY, size, size);

			sprite->setTexture(this->texture);
			sprite->setTextureRect(spriteRect);
			sprite->setScale(scaleFactor);
		}
	}
}

void Game::InitializeTileSprite() {
	// the tile representing the board, they are actually colored as they are drawn
	this->tile.setSize(this->tileSize);
	// the tile representing the tile that a piece can move to
	this->highlight.setSize(this->tileSize);
}

void Game::Update() {
	this->UpdateInput();
}

void Game::UpdateInput() {
	// update mouse position
	this->mousePos =  sf::Mouse::getPosition(*(this->window));
	while (this->window->pollEvent(this->event)) {
		switch (this->event.type) {
			case sf::Event::Closed:
				this->window->close();
				break;
			case sf::Event::MouseButtonPressed:
				if (this->event.mouseButton.button == sf::Mouse::Left) {
					// std::cout << "Clicked" << std::endl;
					this->HandleMouseClick();
				}
				break;
			case sf::Event::MouseButtonReleased:
				if (this->event.mouseButton.button == sf::Mouse::Left) {
					// std::cout << "Released" << std::endl;
					this->HandleMouseRelease();
				}
				break;
			default:
				break;
		}
	}
}

void Game::DrawSquare(sf::RectangleShape square, sf::Vector2f pos, sf::Color color) {
	square.setFillColor(color);
	square.setPosition(pos);
	
	this->window->draw(square);
}

void Game::Render() {
	switch (this->screen) {
		case screens::board: {
			this->window->clear();

			this->RenderBoard();
			this->RenderHighlight();
			this->RenderPiece();
			this->RenderHoldingPiece();
			
			this->window->display();
			break;
		}
		case screens::selection: {
			break;
		}
		default:
			break;
	}
}

void Game::RenderBoard() {
	for (int rank = 0; rank < 8; rank ++) {
		for (int file = 0; file < 8; file ++) {
			bool isLightSquare = ((rank + file) % 2 == 0);
			sf::Color color = (isLightSquare) ? this->lightSquare : this->darkSquare;	

			int x = file * this->tileSize.x;
			int y = std::abs(rank - 7) * this->tileSize.y;
			sf::Vector2f pos (x, y);

			this->DrawSquare(this->tile, pos, color);

		}
	}
}

void Game::RenderHighlight() {
	if (this->board.heldPiece != Piece::none) {
		for (int i : this->board.heldPieceMoves) {			
			int x = this->board.GetIndexFile(i) * this->tileSize.x;
			int y = std::abs(this->board.GetIndexRank(i) - 7) * this->tileSize.y;
			sf::Vector2f pos (x, y);

			this->DrawSquare(this->highlight, pos, this->movableColor);

		}
	}

	if (this->board.lastSquare >= 0 && this->board.nextSquare >= 0) {
		int lastSquareX = this->board.GetIndexFile(this->board.lastSquare) * this->tileSize.x;
		int lastSquareY = std::abs(this->board.GetIndexRank(this->board.lastSquare) - 7) * this->tileSize.y;
	
		int nextSquareX = this->board.GetIndexFile(this->board.nextSquare) * this->tileSize.x;
		int nextSquareY = std::abs(this->board.GetIndexRank(this->board.nextSquare) - 7) * this->tileSize.y;

		sf::Vector2f lastSquarePos (lastSquareX, lastSquareY);
		sf::Vector2f nextSquarePos (nextSquareX, nextSquareY);

		this->DrawSquare(this->highlight, lastSquarePos, this->previousMoveColor);	
		this->DrawSquare(this->highlight, nextSquarePos, this->previousMoveColor);
	}
}

void Game::RenderPiece() {
	for (int rank = 0; rank < 8; rank ++) {
		for (int file = 0; file < 8; file ++) {
			int index = rank * 8 + file;

			int piece = this->board.squares[index];

			// if square isn't empty
			if (piece) {
				int type = Piece::GetType(piece);

				int indexOffset = (Piece::IsColor(piece, Piece::white)) ? 0 : 6;
				int index = this->spriteIndexMap[type] + indexOffset; 

				// a chess board usually starts in the BOTTOM LEFT
				// corner but if SFML coordinates indicates that 0 is the UPPER LEFT
				// so we have to asidhsaiodhasiodh
				int x = file * this->tileSize.x;
				int y = std::abs(rank - 7) * this->tileSize.y;

				sf::Sprite* sprite = &(this->sprites[index]);
				
				sf::Vector2f pos (x, y);
				sprite->setPosition(pos);

				this->window->draw(*(sprite));	
			}
		}
	}
}

void Game::RenderHoldingPiece() {
	int piece = this->board.heldPiece;
	if (piece) {
		int type = Piece::GetType(piece);
		
		int indexOffset = (Piece::IsColor(piece, Piece::white)) ? 0 : 6;
		int index = this->spriteIndexMap[type] + indexOffset;

		sf::Sprite* sprite = & (this->sprites[index]);

		sf::Vector2f pos (this->mousePos.x - this->tileSize.x / 2, this->mousePos.y - this->tileSize.y / 2);
		
		sprite->setPosition(pos);
		this->window->draw(*(sprite));
	}
}

void Game::HandleMouseClick() {
	switch (this->screen) {
		case screens::board: {
			int index = this->GetSquareUnderMouse();
			int piece = (index != -1) ? this->board.squares[index] : Piece::none;
        		
			if (piece) {
				this->lastSquare = index;
        		
				this->board.heldPiece = piece;
				this->board.squares[index] = Piece::none;
        		
				this->board.GenerateHeldPieceMoves(index);
			}
			break;
		}
		case screens::selection: {
			break;
		}
		default:
			break;
	}
}

void Game::HandleMouseRelease() {
	switch (this->screen) {
		case screens::board: {	
			int index = this->GetSquareUnderMouse();
			int piece = this->board.heldPiece;
			
			if (piece) {
				bool canMove = this->board.colorToMove == Piece::GetColor(piece) && this->board.IsMoveValid(index);
				if (canMove) {
					int nextColor = (this->board.colorToMove == Piece::white) ? Piece::black : Piece::white;
				
					this->board.lastSquare = this->lastSquare;
					this->board.nextSquare = index;
                
					this->board.squares[index] = piece;
					this->board.heldPiece = Piece::none;
                
					this->board.colorToMove = nextColor;
					this->board.GeneratePossibleMoves();
				} else {
					this->board.heldPiece = Piece::none;
					this->board.squares[this->lastSquare] = piece;
				}
                
				this->board.ClearHeldPieceMoveData();
			}
		}
		case screens::selection: {
			break;
		}
		default:
			break;
	};
}

int Game::GetSquareUnderMouse() {
	int x = this->mousePos.x / this->tileSize.x;
	int y = this->mousePos.y / this->tileSize.y;

	int index = std::abs(y - 7) * 8 + x;

	// -1 is a fail safe to prevent the player from dragging
	// the mouse out of the window and click and then the game starts to asidsaiojdsaidsajdisjdas itself ok
	return (index >= 0 && index < 64) ? index : -1;
};

bool Game::IsWindowOpen() {
	return this->window->isOpen();
}
