#ifndef GAME_OBJECT_H
#define GAME_OBJECT_H

#include "MathEngine.h"
#include "GraphicsObject.h"
#include "PCSNode.h"

namespace EngineSpace
{
	enum class GOType
	{
		TEXT,
		STATIC,
		DEFAULT,
		BONE
	};

	class GameObject : public PCSNode
	{
	public:
		GameObject() = delete;
		GameObject(const GameObject&) = delete;
		GameObject& operator= (const GameObject&) = delete;
		virtual ~GameObject();

		GameObject(GraphicsObject* pNewObject);

		virtual void Draw();
		virtual void Update() = 0;

		GraphicsObject* getGraphicsObject();
		Matrix* getWorld();
		Vect* getPos();

		void setModelRotate(Matrix* newWorld);
		void setPos(const Vect& pos);
		void setScale(const Vect& scale);
		void setDeltaRot(float delta);
		void toggleSphere();
		void toggleDraw();

		void addBoundingSphere(const char* const pModelFileName);
		
		GOType Type;
	
	protected:
		Matrix* pWorld;
		Matrix* pRot;
		Vect* pTrans;
		GraphicsObject* pGraphicsObject;
		GraphicsObject* pBoundingSphere;
		Vect* pScale;
		float deltaRot;
		float rotY;
		float rotZ;
		bool draw;
		bool boundingSphereActive;

	};
}

#endif