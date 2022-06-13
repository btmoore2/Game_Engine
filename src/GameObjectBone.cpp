#include "GameObjectBone.h"
#include <assert.h>


namespace EngineSpace
{
	GameObjectBone::GameObjectBone(GraphicsObject* newGraphicsObject, BoneData* newBone)
		:GameObject(newGraphicsObject), pBoneData(newBone), boneArrayIndex(0)
	{
		assert(this->pGraphicsObject);
		assert(this->pBoneData);

		this->pBoneOrientation = new Matrix(Matrix::Special::Identity);
		assert(this->pBoneOrientation);

		this->Type = GOType::BONE;
	}


	GameObjectBone::~GameObjectBone()
	{
		delete this->pBoneOrientation;
	}

	void GameObjectBone::privUpdate()
	{
		//All GameObjectBones will be linked to a parent in the skeletal hierearchy, so 
		//position can be set on the top parent and trickle down
		GameObject* pBoneParent = (GameObject*)this->getParent();
		assert(pBoneParent);
		Matrix parentWorld = *pBoneParent->getWorld();

		Matrix Trans = Matrix(Matrix::Trans::XYZ, this->pBoneData[boneArrayIndex].T);
		Matrix Scale = Matrix(Matrix::Scale::XYZ, this->pBoneData[boneArrayIndex].S);
		Quat Rot = this->pBoneData[boneArrayIndex].R;

		//Get the local world
		Matrix localWorld = Scale * Rot * Trans;
		
		//Update world matrix with local & parent
		*this->pWorld = localWorld * parentWorld;
	}

	void GameObjectBone::Update()
	{
		this->privUpdate();

		//Push to the graphics object (This world updated from the animation manager)
		Matrix mTmp = *this->pBoneOrientation;
		this->pGraphicsObject->setWorld(mTmp);
	}

	void GameObjectBone::setIndex(int val)
	{
		this->boneArrayIndex = val;
	}

	int GameObjectBone::getIndex()
	{
		return this->boneArrayIndex;
	}

	void GameObjectBone::setBoneOrientation(const Matrix& tmp)
	{
		*this->pBoneOrientation = tmp;
	}

	Matrix GameObjectBone::getBoneOrientation()
	{
		return Matrix(*this->pBoneOrientation);
	}
}