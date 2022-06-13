#ifndef MOUSE_H
#define MOUSE_H

namespace EngineSpace
{
	//Mouse wrapper for the GLFW functions.
	class Mouse
	{
	public:
		enum Button
		{

		};

		Mouse() = default;
		Mouse(const Mouse&) = delete;
		Mouse& operator= (const Mouse&) = delete;
		~Mouse() = default;

		bool getKeyState(Button button);
		void getCursor(float& xPos, float& yPos);
	};
}


#endif
