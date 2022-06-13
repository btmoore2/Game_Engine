#include "Animation.h"
#include <assert.h>
#include "PCSTreeForwardIterator.h"
#include "GameObjectManager.h"


namespace EngineSpace

{
	Animation::Animation(AnimationClip* newClip, int boneCount, Skeleton::Data* skelData)
		:pClip(newClip),numBones(boneCount)
	{
		this->poBoneResult = new BoneData[(unsigned int)this->numBones];
		assert(this->poBoneResult);

		this->poSkeleton = new Skeleton(this->poBoneResult, this->numBones, skelData);
		assert(this->poSkeleton);

		assert(pClip);
	}

	Animation::~Animation()
	{
		delete[] this->poBoneResult;
		delete this->poSkeleton;
	}

	void Animation::Animate(AnimationTime tCurr)
	{
		this->pClip->animateBones(tCurr);
	}

	void Animation::poseSkeleton(AnimationTime tCurr)
	{
		PCSTreeForwardIterator pIter(this->poSkeleton->getFirstBone());
		PCSNode* pNode = pIter.First();
		GameObjectBone* pGameObj = nullptr;

		int count = 0;

		while (pNode != nullptr)
		{
			assert(pNode);
			// Update the game object
			pGameObj = (GameObjectBone*)pNode;
			privSetBonePose(pGameObj);

			pNode = pIter.Next();
			count++;
		}

		assert(count);
	}


	void Animation::setClip(AnimationClip* newClip)
	{
		this->pClip = newClip;
	}

	AnimationTime Animation::getMaxTime()
	{
		assert(pClip);
		return this->pClip->getTotalTime();
	}

	GameObject* Animation::getSkeleton()
	{
		return this->poSkeleton->getFirstBone();
	}

	void Animation::privSetBonePose(GameObjectBone* pNode)
	{
		assert(pNode);

		GameObjectBone* childNode = pNode;
		GameObjectBone* parentNode; 

		//Catches first level bones which shouldn't be updated by parent.
		if (pNode->getParent()->getParent()->getParent() != nullptr)
		{
			parentNode = (GameObjectBone*)pNode->getParent();
			assert(parentNode);
		}
		else
		{
			childNode->setBoneOrientation(*childNode->getWorld());
			return;
		}

		PCSTree* pTree = GameObjectManager::getPCSTree();
		assert(pTree);

		// starting point
		Vect start(0.0f, 0.0f, 0.0f);

		//  At this point, Find the two bones initial positions in world space
		//  Now get the length and directions

		Vect ptA = start * *parentNode->getWorld();
		Vect ptB = start * *childNode->getWorld();

		// direction between the anchor points of the respective bones
		Vect dir = (ptA - ptB);

		// length of the bone 0
		float mag = dir.mag();

		Matrix S(Matrix::Scale::XYZ, 30.0f, 30.0f, mag);
		Quat Q(Quat::Orient::LocalToWorld, dir.getNorm(), Vect(0.0f, 1.0f, 0.0f));
		Matrix T(Matrix::Trans::XYZ, ptB);

		Matrix BoneOrient = S * Q * T;

		childNode->setBoneOrientation(BoneOrient);
	}


}