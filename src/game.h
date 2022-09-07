#pragma once

#include <SFML/Graphics.hpp>
#include <memory>

#include "board.h"

enum screens {
	board,
	selection
};

class Game {
	public:
		Game();

		// initializers
		void InitializeMembers();
		
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
		int screen = screens::board;

		void InitializeSprites();
		void InitializePieceSprite();
		void InitializeTileSprite();
		void DrawSquare(sf::RectangleShape square, sf::Vector2f pos, sf::Color color);


		void RenderBoard();
		void RenderHighlight();
		void RenderPiece();
		void RenderHoldingPiece();

		void UpdateInput();

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

		/*
		 * apparently you can not write () in classes
		 * but you can write {} to construct it, absolutely no clue
		 */

		sf::Vector2f tileSize { 85.f, 85.f };

		// pieces appearance
		sf::Texture texture;
		sf::Sprite sprites[12];
		std::map<int, int> spriteIndexMap { 
			{Piece::king, 0}	, {Piece::queen, 1},
			{Piece::bishop, 2}	, {Piece::knight, 3},
			{Piece::rook, 4}	, {Piece::pawn, 5}
		};
		
		// tile colors
		sf::Color lightSquare { 140, 162, 173 };
		sf::Color darkSquare { 222, 227, 230 };

		sf::Color previousMoveColor { 255, 182, 86, 70 };
		sf::Color movableColor { 237, 42, 45, 90 };
};
