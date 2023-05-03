#include "game/game.h"

#include <memory>

int main(int argc, char** argv)
{
	std::unique_ptr<Game::GameManager> game = std::make_unique<Game::GameManager>();
	game->Run();
	return 0;
}