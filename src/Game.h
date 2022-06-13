#ifndef GAME_H
#define GAME_H

#include "Engine.h"

namespace EngineSpace
{
	class Game : public Engine
	{
	public:

		Game() = delete;
		Game(const Game&) = delete;
		Game& operator= (const Game&) = delete;
		~Game();

		Game(const char* windowName, const int Width, const int Height);

		const static int SCREEN_WIDTH = 1200;
		const static int SCREEN_HEIGHT = 900;

	private:
		virtual void Initialize() override;
		virtual void loadContent() override;
		virtual void Update(float currentTime) override;
		virtual void Draw() override;
		virtual void unloadContent() override;
		virtual void clearBufferFunc() override;

	};
}

#endif