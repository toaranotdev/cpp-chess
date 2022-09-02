#pragma once

#include <SFML/Graphics.hpp>
#include <memory>

#include "board.h"

class Game {
	public:
		Game();

		// initializers
		void InitializeMembers();
		
		void InitializeWindow();
		
		void InitializeSprites();
		void InitializePieceSprite();
		void InitializeTileSprite();

		void Update();
		void UpdateInput();
		
		void Render();
		void RenderBoard();
		void RenderPiece();
		
		// checks
		bool IsWindowOpen();
	private:
		std::unique_ptr<sf::RenderWindow> window;
		sf::Event event;

		// single square sprite to draw the board
		sf::RectangleShape tile;
		
		/*
			apparently you can do
			sf::Vector2f tileSize (85.f, 85.f) in a FUNCTION
			but not in CLASSES *why*
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

		// our board object
		Board board;

		// tile colors
		sf::Color lightSquare { 140, 162, 173 };
		sf::Color darkSquare { 222, 227, 230 };
};
