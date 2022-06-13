#include "AnimationFrame.h"

namespace EngineSpace
{
	AnimationFrame::AnimationFrame()
		:pNext(nullptr),pPrev(nullptr),keyTime(AnimationTime(AnimationTime::Length::ZERO))
	{
		this->poKeyFrame = new SSBO();
	}

	AnimationFrame::~AnimationFrame()
	{
		delete this->poKeyFrame;
	}

	void AnimationFrame::setTime(AnimationTime newTime)
	{
		this->keyTime = newTime;
	}

	AnimationTime AnimationFrame::getTime()
	{
		return this->keyTime;
	}

	SSBO* AnimationFrame::getFrame()
	{
		return this->poKeyFrame;
	}

}