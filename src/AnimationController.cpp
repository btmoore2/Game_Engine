#include "AnimationController.h"
#include <assert.h>

namespace EngineSpace
{
	AnimationController::AnimationController(AnimationTime delta, AnimationClip* pClip, int numBones, Skeleton::Data* skelData)
		:pNext(nullptr), currStatus(Status::LOADING), pClipHead(pClip), pCurrClip(pClip), poTransition(nullptr)
	{
		this->poAnimation = new Animation(pClip, numBones, skelData);
		assert(this->poAnimation);

		this->poTimer = new AnimationTimer(delta, AnimationTime(AnimationTime::Length::ZERO), poAnimation->getMaxTime());
		assert(this->poTimer);
	}

	AnimationController::~AnimationController()
	{
		delete this->poAnimation;
		delete this->poTimer;
		delete this->poTransition;

		AnimationClip* pDel = this->pClipHead;
		AnimationClip* pDelNext;

		while (pDel != nullptr)
		{
			pDelNext = pDel->pNext;
			delete pDel;
			pDel = pDelNext;
		}
	}

	void AnimationController::addClip(AnimationClip* newClip)
	{
		newClip->pNext = this->pClipHead;
		this->pClipHead = newClip;
		this->pCurrClip = newClip;
	}

	void AnimationController::nextClip()
	{
		AnimationClip* pastClip = this->pCurrClip;
		this->pCurrClip = this->pCurrClip->pNext;
		if (this->pCurrClip == nullptr)
		{
			this->pCurrClip = this->pClipHead;
		}
		this->poAnimation->setClip(this->pCurrClip);
		
		//delete the old just in case flipping through animations quickly
		delete this->poTransition;
		this->poTransition = new AnimationTransition(pastClip, this->pCurrClip);		
	}

	void AnimationController::Reverse()
	{
		this->poTimer->Reverse();
	}

	void AnimationController::Faster()
	{
		AnimationTime currDelta = this->poTimer->getDeltaTime();
		currDelta += (0.1f * AnimationTime(AnimationTime::Length::FPS30));
		this->poTimer->setDeltaTime(currDelta);
	}

	void AnimationController::Slower()
	{
		AnimationTime currDelta = this->poTimer->getDeltaTime();
		currDelta -= (0.1f * AnimationTime(AnimationTime::Length::FPS30));
		this->poTimer->setDeltaTime(currDelta);
	}

	void AnimationController::Update()
	{
		this->poTimer->Update();

		//Check if we're transitioning between clips.
		if (this->poTransition != nullptr)
		{
			if (this->poTransition->isDone() == false)
			{
				this->poTransition->Update(this->poTimer->getCurrTime());
			}
			else
			{
				delete this->poTransition;
				this->poTransition = nullptr;
			}
		}
		else
		{
			//Update skeleton position and update them.
			this->poAnimation->Animate(this->poTimer->getCurrTime());
			this->poAnimation->poseSkeleton(this->poTimer->getCurrTime());
		}
	}

	GameObject* AnimationController::getSkeleton() const
	{
		return this->poAnimation->getSkeleton();
	}
}