#include "GameManager.h"
#include "Mouse.h"

namespace EngineSpace
{

	bool Mouse::getKeyState(Button button)
	{
		if (GameManager::getGame()->getMouseButton(button) == GLFW_PRESS)
		{
			return true;
		}
		
		return false;
	}

	void Mouse::getCursor(float& xPos, float& yPos)
	{
		GameManager::getGame()->getCursorPos(xPos, yPos);
	}
}