#include "game.h"

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
		this->texture.setSmooth(true);
		
		int size = 334; // size of one sprite in that whole sprite atlas
		sf::Vector2f scaleFactor (85.f / 334.f, 85.f / 334.f);
		
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
	// square size, the tiles are actually colored as they are drawn
	this->tile.setSize(this->tileSize);
}

void Game::Update() {
	this->UpdateInput();
}

void Game::UpdateInput() {
	while (this->window->pollEvent(this->event)) {
		switch (this->event.type) {
			case sf::Event::Closed:
				this->window->close();
				break;
			default:
				break;
		}
	}
}

void Game::Render() {
	this->window->clear();
	this->RenderBoard();
	this->RenderPiece();
	this->window->display();
}

void Game::RenderBoard() {
	for (int file = 0; file < 8; file ++) {
		for (int rank = 0; rank < 8; rank ++) {
			bool isLightSquare = ((rank + file) % 2 == 0);
			
			sf::Color squareColor = (isLightSquare) ? this->lightSquare : this->darkSquare;
			this->tile.setFillColor(squareColor);
		

			int x = file * this->tileSize.x;
			int y = rank * this->tileSize.y;
			this->tile.setPosition(sf::Vector2f(x, y));
			this->window->draw(this->tile);
		}
	}
}

void Game::RenderPiece() {
	for (int file = 0; file < 8; file ++) {
		for (int rank = 0; rank < 8; rank ++) {
			int index = rank * 8 + file;

			int piece = this->board.squares[index];

			// if square isn't empty
			if (piece) {
				int color = Piece::GetColor(piece);
				int type = Piece::GetType(piece);

				int indexOffset = (color == Piece::white) ? 0 : 6;
				int index = this->spriteIndexMap[type] + indexOffset; 

				int x = file * this->tileSize.x;
				int y = rank * this->tileSize.y;

				sf::Sprite* sprite = &(this->sprites[index]);
				
				sf::Vector2f position (x, y);
				sprite->setPosition(position);

				this->window->draw(*(sprite));	
			}
		}
	}
}

bool Game::IsWindowOpen() {
	return this->window->isOpen();
}
