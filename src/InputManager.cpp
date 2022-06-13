#include <assert.h>
#include "InputManager.h"

namespace EngineSpace
{

	void InputManager::Create()
	{
		InputManager* pInputManager = InputManager::privGetInstance();
		assert(pInputManager);
	}

	void InputManager::Destroy()
	{
		InputManager* pMan = InputManager::privGetInstance();
		assert(pMan);

		delete pMan;
	}

	InputManager::InputManager()
	{
		this->poKeyboard = new Keyboard();
		assert(this->poKeyboard);
		this->poMouse = new Mouse();
		assert(this->poMouse);
	}

	InputManager::~InputManager()
	{
		delete this->poKeyboard;
		delete this->poMouse;
	}


	Keyboard* InputManager::getKeyboard()
	{
		InputManager* pMan = InputManager::privGetInstance();
		assert(pMan);

		return pMan->poKeyboard;
	}

	Mouse* InputManager::getMouse()
	{
		InputManager* pMan = InputManager::privGetInstance();
		assert(pMan);

		return pMan->poMouse;
	}

	InputManager* InputManager::privGetInstance()
	{
		static InputManager singleManager;
		return &singleManager;
	}
}