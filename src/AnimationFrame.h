#ifndef ANIMATION_FRAME_H
#define ANIMATION_FRAME_H

#include "SSBO.h"
#include "AnimationTime.h"

namespace EngineSpace
{
	class AnimationFrame
	{
	public:
		AnimationFrame();
		AnimationFrame(const AnimationFrame&) = delete;
		AnimationFrame& operator= (const AnimationFrame&) = delete;
		~AnimationFrame();

		AnimationTime getTime();
		void setTime(AnimationTime newTime);
		SSBO* getFrame();

		AnimationFrame* pNext;
		AnimationFrame* pPrev;

	private:

		AnimationTime keyTime;
		SSBO* poKeyFrame;

	};
}

#endif
