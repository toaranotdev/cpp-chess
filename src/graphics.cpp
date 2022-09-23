#include "graphics.h"

namespace Graphics {
	sf::Sprite pieceSprites[12];
	sf::Texture spriteTexture;
	sf::Texture selectionTexture;
	
	sf::Vector2f tileSize { 85.f, 85.f };
	sf::RectangleShape tile { Graphics::tileSize };
	sf::RectangleShape activeSelection { Graphics::tileSize };
	sf::RectangleShape inactiveSelection { Graphics::tileSize };

	std::map<int, int> pieceSpriteIndexMap { 
		{Piece::king, 0}	, {Piece::queen, 1},
		{Piece::bishop, 2}	, {Piece::knight, 3},
		{Piece::rook, 4}	, {Piece::pawn, 5}
	};
};

void Graphics::Initialize() {
	float size = 334; // size of one sprite in that whole sprite atlas
	
	if (!Graphics::spriteTexture.loadFromFile("./assets/textures/pieces.png")) {
		std::cout << "FileNotFoundError: 	Can't load ./assets/textures/pieces.png." << std::endl;
		std::cout << "Are you running the binary from the project folder?" << std::endl;
	} else {
		Graphics::spriteTexture.setSmooth(true);
		sf::Vector2f scaleFactor { Graphics::tileSize.x / size, Graphics::tileSize.y / size };
		
		for (int i = 0; i < 12; i ++) {
			// pointers woo, we don't have to delete this, since 
			// we did not ask for memory (i.e: using new keyword)
			sf::Sprite* sprite = &(Graphics::pieceSprites[i]);

			// epic math to figure out where to cut the texture and shit
			int shiftX = (i > 6) ? (i - 6) * size : i * size;
			int shiftY = (i > 6) ?  size : 0;

			sf::IntRect spriteRect (shiftX, shiftY, size, size);

			sprite->setTexture(Graphics::spriteTexture);
			sprite->setTextureRect(spriteRect);
			sprite->setScale(scaleFactor);
		}
	}

	if (!Graphics::selectionTexture.loadFromFile("./assets/textures/selection.png")) {
		std::cout << "FileNotFoundError: 	Can't load ./assets/textures/selection.png" << std::endl;
		std::cout << "Are you running the binary from the project folder?" << std::endl;
	} else {
		const float size = 85.f * 1.5;

		Graphics::activeSelection.setTexture(&(Graphics::selectionTexture));
		Graphics::activeSelection.setTextureRect({ 0, 0, 334, 334 });
		Graphics::activeSelection.setSize({ size, size });

		Graphics::inactiveSelection.setTexture(&(Graphics::selectionTexture));
		Graphics::inactiveSelection.setTextureRect({ 334, 0, 334, 334 });
		Graphics::inactiveSelection.setSize({ size, size });
	}
}

void Graphics::DrawSquare(sf::Vector2f pos, sf::Color color, sf::RenderTarget* target) {
	Graphics::tile.setPosition(pos);
	Graphics::tile.setFillColor(color);

	target->draw(Graphics::tile);
}
