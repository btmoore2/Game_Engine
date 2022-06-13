#ifndef ANIMATION_CONTROLLER_H
#define ANIMATION_CONTROLLER_H

#include "Animation.h"
#include "AnimationTransition.h"
#include "AnimationTimer.h"

namespace EngineSpace
{
	enum class Status
	{
		LOADING,
		FINISHED
	};

	class AnimationController
	{
	public:
		AnimationController() = delete;
		AnimationController(const AnimationController&) = delete;
		AnimationController& operator= (const AnimationController&) = delete;
		~AnimationController();

		AnimationController(AnimationTime delta, AnimationClip* pClip, int numBones, Skeleton::Data* skelData);

		void addClip(AnimationClip* newClip);
		void nextClip();
		void Reverse();
		void Faster();
		void Slower();
		void Update();

		GameObject* getSkeleton() const;

		AnimationController* pNext;
		Status currStatus;
	private:
		AnimationTimer* poTimer;
		AnimationClip* pClipHead;
		AnimationClip* pCurrClip;
		Animation* poAnimation;
		AnimationTransition* poTransition;


	};
}


#endif
