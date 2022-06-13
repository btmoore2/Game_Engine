#include "GameManager.h"
#include "Keyboard.h"

namespace EngineSpace
{
	bool Keyboard::getKeyState(Key key)
	{
		if (GameManager::getGame()->getKey(key) == GLFW_PRESS)
		{
			return true;
		}

		return false;
	}
}