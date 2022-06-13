#include <Windows.h>

#include "GameManager.h"

using namespace EngineSpace;

int CALLBACK WinMain(HINSTANCE, HINSTANCE, LPSTR, int)
{
	GameManager::Create("Demo", Game::SCREEN_WIDTH, Game::SCREEN_HEIGHT);

	Game* pGame = GameManager::getGame();
	pGame->Run();

	GameManager::Destroy();

	return 0;
}