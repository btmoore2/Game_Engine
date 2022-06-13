#ifndef GAME_MANAGER_H
#define GAME_MANAGER_H

#include "Game.h"

namespace EngineSpace
{
	class GameManager
	{
	public:
		GameManager() = delete;
		GameManager(const GameManager&) = delete;
		GameManager& operator= (const GameManager&) = delete;

		static void Create(const char* windowName, const int Width, const int Height);
		static void Destroy();

		static Game* getGame();

	private:
		GameManager(Game* pGame);
		~GameManager();

		static GameManager* pInstance;
		static void privCreate(const char* windowName, const int Width, const int Height);

		static GameManager* privInstance();

		Game* poGame;
	};
}

#endif