#pragma once
#include <memory>

#include "board.h"
#include "graphics.h"

enum screens {
	BOARD,
	SELECTION
};

class Game {
	public:
		Game();
		
		void InitializeWindow();
		void Update();
		void Render();

		void HandleMouseClick();
		void HandleMouseRelease();

		void ChangeScreen(int screen);

		// getters
		int GetSquareUnderMouse();
		
		// checks
		bool IsWindowOpen();
	private:
		void RenderBoard();
		void RenderHighlight();
		void RenderPiece();
		void RenderHoldingPiece();

		void RenderOverlay();
		void RenderSelection();

		void UpdateInput();

		void HandleMove();

		std::unique_ptr<sf::RenderWindow> window;
		sf::Event event;
		// our board object
		Board board;
		// mouse position
		sf::Vector2i mousePos;
		// single square sprite to draw the board
		sf::RectangleShape tile;
		// single square sprite to draw highlights
		sf::RectangleShape highlight;
		// contains the previously clicked square
		int lastSquare;
		// screen
		int screen = screens::BOARD;
};
