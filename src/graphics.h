#pragma once
#include <iostream>
#include <memory>

#include "piece.h"
#include "theme.h"

namespace Graphics {
	extern sf::Sprite pieceSprites[];

	extern sf::Texture spriteTexture;
	extern sf::Texture selectionTexture;

	extern sf::Vector2f tileSize;

	extern sf::RectangleShape tile;
	extern sf::RectangleShape inactiveSelection;
	extern sf::RectangleShape activeSelection;

	extern std::map<int, int > pieceSpriteIndexMap;

	void Initialize();

	void DrawSquare(sf::Vector2f pos, sf::Color color, sf::RenderTarget* target);
};
