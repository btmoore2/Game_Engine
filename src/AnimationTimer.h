#ifndef ANIMATION_TIMER_H
#define ANIMATION_TIMER_H

#include "AnimationTime.h"

namespace EngineSpace
{
	class AnimationTimer
	{
	public:
		AnimationTimer() = delete;
		AnimationTimer(const AnimationTimer&) = delete;
		AnimationTimer& operator= (const AnimationTimer&) = delete;
		~AnimationTimer();

		AnimationTimer(AnimationTime delta, AnimationTime start, AnimationTime end);

		void Update();
		void Reverse();

		void setCurrTime(const AnimationTime time);
		void setDeltaTime(const AnimationTime time);
		void setStartTime(const AnimationTime time);
		void setEndTime(const AnimationTime time);
		AnimationTime getCurrTime() const;
		AnimationTime getDeltaTime() const;
		AnimationTime getStartTime() const;
		AnimationTime getEndTime() const;

	private:
		AnimationTime current;
		AnimationTime delta;
		AnimationTime start;
		AnimationTime end;


	};
}


#endif
