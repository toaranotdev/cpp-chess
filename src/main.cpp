#include "game.h"

int main () {
	Game game;

	while (game.IsWindowOpen()) {
		game.Update();
		game.Render();
	}

	return 0;
}
