#ifndef INPUT_MANAGER_H
#define INPUT_MANAGER_H

#include "Mouse.h"
#include "Keyboard.h"

namespace EngineSpace
{
	class InputManager
	{
	public:
		InputManager(const InputManager&) = delete;
		InputManager& operator= (const InputManager&) = delete;
		~InputManager();

		static void Create();
		static void Destroy();

		static Keyboard* getKeyboard();
		static Mouse* getMouse();

	private:
		InputManager();		
		static InputManager* privGetInstance();

		Keyboard* poKeyboard;
		Mouse* poMouse;

	};
}


#endif
