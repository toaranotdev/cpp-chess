#include "game.h"
#include "piece.h"
#include <cmath>
#include <iostream>

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
	this->highlight.setFillColor(sf::Color(237, 42, 45, 90));
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

void Game::Render() {
	this->window->clear();
	this->RenderBoard();
	this->RenderHighlight();
	this->RenderPiece();
	this->RenderHoldingPiece();
	this->window->display();
}

void Game::RenderBoard() {
	for (int rank = 0; rank < 8; rank ++) {
		for (int file = 0; file < 8; file ++) {
			bool isLightSquare = ((rank + file) % 2 == 0);
			
			sf::Color squareColor = (isLightSquare) ? this->lightSquare : this->darkSquare;
			this->tile.setFillColor(squareColor);
		

			int x = file * this->tileSize.x;
			int y = std::abs(rank - 7) * this->tileSize.y;

			sf::Vector2f position (x, y);

			this->tile.setPosition(position);
			this->window->draw(this->tile);
		}
	}
}

void Game::RenderHighlight() {
	if (this->board.heldPiece != Piece::none) {
		for (int i : this->board.heldPieceMoves) {
			
			int x = this->board.GetIndexFile(i) * this->tileSize.x;
			int y = std::abs(this->board.GetIndexRank(i) - 7) * this->tileSize.y;

			sf::Vector2f position (x, y);

			this->highlight.setPosition(position);
			this->window->draw(this->highlight);
		}
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
				
				sf::Vector2f position (x, y);
				sprite->setPosition(position);

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

		sf::Vector2f position (this->mousePos.x - this->tileSize.x / 2, this->mousePos.y - this->tileSize.y / 2);
		
		sprite->setPosition(position);
		this->window->draw(*(sprite));
	}
}

void Game::HandleMouseClick() {
	int index = this->GetSquareUnderMouse();
	int piece = (index != -1) ? this->board.squares[index] : Piece::none;

	if (piece) {
		this->lastSquare = index;

		this->board.heldPiece = piece;
		this->board.squares[index] = Piece::none;

		this->board.GenerateHeldPieceMoves(index);
	}
}

void Game::HandleMouseRelease() {
	int index = this->GetSquareUnderMouse();
	int piece = this->board.heldPiece;
	
	if (piece) {
		bool canMove = this->board.colorToMove == Piece::GetColor(piece) && this->board.IsMoveValid(index);
		if (canMove) {
			int nextColor = (this->board.colorToMove == Piece::white) ? Piece::black : Piece::white;
			
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
