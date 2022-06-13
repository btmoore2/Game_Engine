#ifndef GAME_OBJECT_STATIC_H
#define GAME_OBJECT_STATIC_H

#include "GameObject.h"
#include "MathEngine.h"
#include "GraphicsObject.h"

namespace EngineSpace
{
	class GameObjectStatic : public GameObject
	{
	public:
		GameObjectStatic() = delete;
		GameObjectStatic(const GameObjectStatic&) = delete;
		GameObjectStatic& operator= (const GameObjectStatic&) = delete;

		GameObjectStatic(GraphicsObject* newGraphicsObject);

		virtual void Update();

	private:
		void privUpdate();

	};
}

#endif
