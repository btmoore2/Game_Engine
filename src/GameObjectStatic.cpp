#include "GameObjectStatic.h"

namespace EngineSpace
{
	GameObjectStatic::GameObjectStatic(GraphicsObject* newGraphicsObject)
		:GameObject(newGraphicsObject)
	{
		this->Type = GOType::STATIC;
	}

	void GameObjectStatic::Update()
	{
		//Update world matrix and push to the graphics object
		this->privUpdate();

		this->pGraphicsObject->setWorld(*this->pWorld);
	}

	void GameObjectStatic::privUpdate()
	{
		this->rotY += deltaRot;

		Matrix Scale(Matrix::Scale::XYZ, *this->pScale);
		Matrix TransA(Matrix::Trans::XYZ, *this->pTrans);
		Matrix RotY(Matrix::Rot1::Y, this->rotY);
		Matrix RotZ(Matrix::Rot1::Z, this->rotZ);

		*this->pWorld = Scale * *this->pRot * RotY * RotZ * TransA;
	}
}