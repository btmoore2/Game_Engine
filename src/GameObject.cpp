#include <assert.h>
#include "GameObject.h"

namespace EngineSpace
{
	GameObject::GameObject(GraphicsObject* pNewObject)
		:Type(GOType::DEFAULT),pGraphicsObject(pNewObject), pBoundingSphere(nullptr), deltaRot(0.0f), rotY(0.0f), rotZ(0.0f), draw(true), boundingSphereActive(false)
	{
		this->pWorld = new Matrix(Matrix::Special::Identity);
		assert(pWorld);
		this->pRot = new Matrix(Matrix::Special::Identity);
		assert(pRot);
		this->pTrans = new Vect(0.0f, 0.0f, 0.0f);
		assert(pTrans);
		this->pScale = new Vect(1.0f, 1.0f, 1.0f);
		assert(pScale);
	}

	GameObject::~GameObject()
	{
		delete this->pWorld;
		delete this->pRot;
		delete this->pTrans;
		delete this->pScale;
		delete this->pGraphicsObject;
		delete this->pBoundingSphere;
	}

	Matrix* GameObject::getWorld()
	{
		return this->pWorld;
	}

	Vect* GameObject::getPos()
	{
		return this->pTrans;
	}

	GraphicsObject* GameObject::getGraphicsObject()
	{
		return this->pGraphicsObject;
	}

	void GameObject::setModelRotate(Matrix* newWorld)
	{
		assert(newWorld);
		*this->pRot = *newWorld;
	}

	void GameObject::setPos(const Vect& pos)
	{
		*this->pTrans = pos;
	}

	void GameObject::setScale(const Vect& newScale)
	{
		*this->pScale = newScale;
	}

	void GameObject::setDeltaRot(float delta)
	{
		this->deltaRot = delta;
	}

	void GameObject::Draw()
	{
		if (this->draw == true)
		{
			this->pGraphicsObject->Render();
		}

		if (this->boundingSphereActive == true)
		{
			this->pBoundingSphere->Render();
		}
	}

	void GameObject::toggleSphere()
	{
		if (this->pBoundingSphere != nullptr)
		{
			this->boundingSphereActive = !this->boundingSphereActive;
		}
	}

	void GameObject::toggleDraw()
	{
		this->draw = !this->draw;
	}

	void GameObject::addBoundingSphere(const char* const pModelFileName)
	{
		UNUSED_VAR(pModelFileName);
		//BoundingSphere* pSphere = new BoundingSphere(pModelFileName);
		//this->pBoundingSphere = new WireframeGraphicsObject(pSphere, ShaderManager::Find(ShaderObject::Name::CONST_COLOR));
		//this->BoundingSphereActive = true;
	}

}