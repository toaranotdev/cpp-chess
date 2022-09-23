#include "game.h"
#include "graphics.h"
#include "theme.h"

Game::Game() {
	Graphics::Initialize();

	this->InitializeWindow();
}

void Game::InitializeWindow() {
	std::string title = "Chess"; // window title
	sf::VideoMode mode = sf::VideoMode(680, 680); // window size, etc...

	this->window = std::unique_ptr<sf::RenderWindow>(new sf::RenderWindow(mode, title));
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
	switch (this->screen) {
		case screens::BOARD: {
			this->window->clear();

			this->RenderBoard();
			this->RenderHighlight();
			this->RenderPiece();
			this->RenderHoldingPiece();
			
			this->window->display();
			break;
		}
		case screens::SELECTION: {
			this->window->clear();

			this->RenderBoard();
			this->RenderPiece();
			this->RenderOverlay();
			this->RenderSelection();

			this->window->display();
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
			sf::Color color = (isLightSquare) ? Theme::lightSquareCol : Theme::darkSquareCol;	

			int x = file * Graphics::tileSize.x;
			int y = std::abs(rank - 7) * Graphics::tileSize.y;
			sf::Vector2f pos (x, y);

			Graphics::DrawSquare(pos, color, this->window.get());

		}
	}
}

void Game::RenderHighlight() {
	// legal move highlights
	if (this->board.heldPiece != Piece::none) {
		for (int i : this->board.heldPieceMoves) {			
			int x = this->board.GetIndexFile(i) * Graphics::tileSize.x;
			int y = std::abs(this->board.GetIndexRank(i) - 7) * Graphics::tileSize.y;
			sf::Vector2f pos (x, y);
			
			Graphics::DrawSquare(pos, Theme::legalMoveCol, this->window.get());
		}
	}

	if (this->board.lastSquare >= 0 && this->board.nextSquare >= 0) {
		int lastSquareX = this->board.GetIndexFile(this->board.lastSquare) * Graphics::tileSize.x;
		int lastSquareY = std::abs(this->board.GetIndexRank(this->board.lastSquare) - 7) * Graphics::tileSize.y;
	
		int nextSquareX = this->board.GetIndexFile(this->board.nextSquare) * Graphics::tileSize.x;
		int nextSquareY = std::abs(this->board.GetIndexRank(this->board.nextSquare) - 7) * Graphics::tileSize.y;

		sf::Vector2f lastSquarePos (lastSquareX, lastSquareY);
		sf::Vector2f nextSquarePos (nextSquareX, nextSquareY);

		Graphics::DrawSquare(lastSquarePos, Theme::lastMoveCol, this->window.get());	
		Graphics::DrawSquare(nextSquarePos, Theme::lastMoveCol, this->window.get());
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
				int index = Graphics::pieceSpriteIndexMap[type] + indexOffset; 

				// a chess board usually starts in the BOTTOM LEFT
				// corner but if SFML coordinates indicates that 0 is the UPPER LEFT
				// so we have to asidhsaiodhasiodh
				int x = file * Graphics::tileSize.x;
				int y = std::abs(rank - 7) * Graphics::tileSize.y;

				sf::Sprite* sprite = &(Graphics::pieceSprites[index]);
				
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
		int index = Graphics::pieceSpriteIndexMap[type] + indexOffset;

		sf::Sprite* sprite = & (Graphics::pieceSprites[index]);

		sf::Vector2f pos (this->mousePos.x - Graphics::tileSize.x / 2, this->mousePos.y - Graphics::tileSize.y / 2);
		
		sprite->setPosition(pos);
		this->window->draw(*(sprite));
	}
}

void Game::RenderOverlay() {
	sf::Vector2f originalSize = Graphics::tile.getSize();
	sf::Vector2<unsigned int> windowSize = this->window->getSize();

	sf::Color originalColor = Graphics::tile.getFillColor();
	sf::Color color { 0, 0, 0, 100 };

	Graphics::tile.setSize (sf::Vector2f( windowSize.x, windowSize.y ));
	Graphics::tile.setPosition (sf::Vector2f(0.f, 0.f));
	Graphics::tile.setFillColor(color);

	this->window->draw (Graphics::tile);
	
	Graphics::tile.setSize (originalSize);
	Graphics::tile.setFillColor(originalColor);
}

void Game::RenderSelection() {
	const int pieceList[] = { Piece::bishop, Piece::knight, Piece::rook, Piece::queen };
	int indexOffset = (Piece::IsColor(this->board.heldPiece, Piece::white)) ? 0 : 6;
	float size = 85.f * 1.5;
	sf::Vector2<unsigned int> windowSize = this->window->getSize();
	// super extra complicated math, this is probably the smartest equation
	// i have ever fkin came up with lmao	
	float startingX = (((windowSize.x / size) - 4) / 2) * size;
	
	// had to subtract because the sprite origin is in the top left instead of in the center
	const float y = (windowSize.y / size / 2.f) * size - (size / 2.f);

	for (int i = 0; i < 4; i ++) {
		int piece = pieceList[i];
		int pieceIndex = Graphics::pieceSpriteIndexMap[piece] + indexOffset;
		float finalX = startingX + size * i;

		sf::Vector2f pos { finalX, y };

		sf::Sprite* sprite = &(Graphics::pieceSprites[pieceIndex]);		
		
		// sprite stuff
		sf::Vector2f originalScale = sprite->getScale();
		sf::Vector2f scale { size / 334, size / 334 };

		sprite->setPosition(pos);
		sprite->setScale(scale);

		// the square background highlight thingy
		Graphics::inactiveSelection.setPosition(pos);
		Graphics::activeSelection.setPosition(pos);

		bool isMouseHoverred = (Graphics::inactiveSelection.getGlobalBounds().contains(this->mousePos.x, this->mousePos.y));
		sf::RectangleShape* selectionBg = &((isMouseHoverred) ? Graphics::activeSelection : Graphics::inactiveSelection);

		// finally, we draw shit	
		this->window->draw(*(selectionBg));
		this->window->draw(*(sprite));

		// reset the scale so that it doesn't fuck itself when drawing the board
		sprite->setScale(originalScale);
	}
}

/*
void Game::RenderSelection() {
	int pieceList[] = { Piece::bishop, Piece::knight, Piece::rook, Piece::queen };
	sf::Vector2<unsigned int> windowSize = this->window->getSize();
	
	float size = 85.f * 1.5;
	// super extra complicated math, this is probably the smartest equation
	// i have ever fkin came up with lmao	
	float startingX = (((windowSize.x / size) - 4) / 2) * size;
	
	// had to subtract because the sprite origin is in the top left instead of in the center
	const float y = (windowSize.y / size / 2.f) * size - (size / 2.f);

	for (int i = 0; i < 4; i ++) {
		float finalX = startingX + size * i;
		
		sf::Vector2f pos { finalX, y };
		Graphics::inactiveSelection.setPosition(pos);
		Graphics::activeSelection.setPosition(pos);
		
		if (Graphics::activeSelection.getGlobalBounds().contains(this->mousePos.x, this->mousePos.y)) {
			this->window->draw(Graphics::activeSelection);
			this->board.heldPiece = (Piece::GetColor(this->board.heldPiece)) | pieceList[i];
		} else {
			this->window->draw(Graphics::inactiveSelection);
		}
	}
}*/

void Game::HandleMouseClick() {
	switch (this->screen) {
		case screens::BOARD: {
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
		case screens::SELECTION: {
			sf::Vector2<unsigned int> windowSize = this->window->getSize();

			const int pieceList[] = { Piece::bishop, Piece::knight, Piece::rook, Piece::queen };
			const float size = 85.f * 1.5; 
			const float startingX = (((windowSize.x / size) - 4) / 2) * size;
			// these values determines the region that the selection sprites (Graphics::inactiveSelection and Graphics::activeSelection)
			// occupy
			const float firstX = startingX;
			const float secondX = startingX + size * 4; // multiply it by 4 because... there are 4 squares xd
			const float firstY = (windowSize.y / size / 2.f) * size - (size / 2.f);
			const float secondY = firstY + size;

			if (this->mousePos.x > firstX && this->mousePos.x < secondX && this->mousePos.y > firstY && this->mousePos.y < secondY) {
				// not really sure how this works, took some trials and errors to make this lol
				float selectionIndex = (this->mousePos.x - startingX) / size;
				int color = this->board.colorToMove;
				int startSquare = this->lastSquare;
				int targetSquare = *(this->board.heldPieceMoves.begin());
                        
				this->board.heldPiece = pieceList[int(selectionIndex)] | color;
				this->board.squares[targetSquare] = this->board.heldPiece;
				this->board.heldPiece = Piece::none;
                        
				this->ChangeScreen(screens::BOARD);
                        
				this->board.lastSquare = startSquare;
				this->board.nextSquare = targetSquare;
                        
				this->board.colorToMove = (this->board.colorToMove == Piece::black) ? Piece::white : Piece::black;
				this->board.GeneratePossibleMoves();
				this->board.ClearHeldPieceMoveData();
			}
			break;
		}
		default:
			break;
	}
}

void Game::HandleMouseRelease() {
	switch (this->screen) {
		case screens::BOARD: {	
			int startSquare = this->lastSquare;
			int targetSquare = this->GetSquareUnderMouse();
			int piece = this->board.heldPiece;
				
			if (piece) {
				bool canMove = this->board.colorToMove == Piece::GetColor(piece) && this->board.IsMoveValid(targetSquare);	
				if (canMove) {
					int flag = this->board.GetMoveFlag(startSquare, targetSquare);
					int nextColor = (this->board.colorToMove == Piece::white) ? Piece::black : Piece::white;

					switch (flag) {
						case Flag::DOUBLE_PUSH: {
							this->board.enPassantFile = this->board.GetIndexFile(targetSquare);
							break;
						}
						case Flag::PROMOTION: {
							this->ChangeScreen(screens::SELECTION);
							return;
						}
						case Flag::EN_PASSANT: {
							// we need to grab the square behind it so
							// this is the opposite of what you usually see
							int directionOffset = (Piece::IsColor(piece, Piece::white)) ? -1 : 1;
							int squareBehind = targetSquare	+ 8 * directionOffset;
							this->board.squares[squareBehind] = Piece::none;
							break;
						}
						default:
							break;
					}
 
					this->board.lastSquare = startSquare;
					this->board.nextSquare = targetSquare;
					
					this->board.squares[targetSquare] = piece;
					this->board.heldPiece = Piece::none;
                
					this->board.colorToMove = nextColor;
					this->board.GeneratePossibleMoves();
				} else {
					this->board.heldPiece = Piece::none;
					this->board.squares[this->lastSquare] = piece;
				}
                
				this->board.ClearHeldPieceMoveData();
			}
			break;
		}
		case screens::SELECTION: {
			break;
		}
		default:
			break;
	};
}

void Game::ChangeScreen(int screen) {
	this->screen = screen;
}

int Game::GetSquareUnderMouse() {
	int x = this->mousePos.x / Graphics::tileSize.x;
	int y = this->mousePos.y / Graphics::tileSize.y;

	int index = std::abs(y - 7) * 8 + x;

	// -1 is a fail safe to prevent the player from dragging
	// the mouse out of the window and click and then the game starts to asidsaiojdsaidsajdisjdas itself ok
	return (index >= 0 && index < 64) ? index : -1;
}

bool Game::IsWindowOpen() {
	return this->window->isOpen();
}
