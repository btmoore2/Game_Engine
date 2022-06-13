#include "AnimationTimer.h"

namespace EngineSpace
{
	AnimationTimer::AnimationTimer(AnimationTime newDelta, AnimationTime newStart, AnimationTime newEnd)
		:current(AnimationTime::Length::ZERO), delta(newDelta), start(newStart), end(newEnd)
	{

	}

	AnimationTimer::~AnimationTimer()
	{

	}

	void AnimationTimer::Update()
	{
		this->current += this->delta;

		//These checks allow forward/backwards looping. 
		if (this->current < this->start)
		{
			this->current = this->end;
		}
		else if (this->current > this->end)
		{
			this->current = this->start;
		}
	}

	void AnimationTimer::Reverse()
	{
		this->delta = -this->delta;
	}

	void AnimationTimer::setCurrTime(AnimationTime time)
	{
		this->current = time;
	}

	void AnimationTimer::setDeltaTime(AnimationTime time)
	{
		this->delta = time;
	}

	void AnimationTimer::setStartTime(AnimationTime time)
	{
		this->start = time;
	}

	void AnimationTimer::setEndTime(AnimationTime time)
	{
		this->end = time;
	}

	AnimationTime AnimationTimer::getCurrTime() const
	{
		return this->current;
	}

	AnimationTime AnimationTimer::getDeltaTime() const
	{
		return this->delta;
	}

	AnimationTime AnimationTimer::getStartTime() const
	{
		return this->start;
	}

	AnimationTime AnimationTimer::getEndTime() const
	{
		return this->end;
	}


}