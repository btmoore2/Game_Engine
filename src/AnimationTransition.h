#ifndef ANIMATION_TRANSITION_H
#define ANIMATION_TRANSITION_H

#include "AnimationClip.h"

namespace EngineSpace
{

	class AnimationTransition
	{
	public:
		AnimationTransition() = delete;
		AnimationTransition(const AnimationTransition&) = delete;
		AnimationTransition& operator= (const AnimationTransition&) = delete;
		~AnimationTransition();

		AnimationTransition(AnimationClip* pCurrent, AnimationClip* pNext);

		bool isDone();
		void Update(AnimationTime tCurr);

	private:
		void privTransitionTime();

		AnimationClip* pCurrClip;
		AnimationClip* pNextClip;
		SSBO* poResult;
		float t;
		float tWeight;
		int numBones;
		bool complete;

	};

}

#endif


