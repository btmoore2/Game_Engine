#ifndef ANIMATION_TIME_H
#define ANIMATION_TIME_H

namespace EngineSpace
{

	class AnimationTime
	{
	public: 
		enum class Length
		{
			ZERO,
			MILLISECOND,
			SECOND,
			MINUTE,
			FPS30,
			FPS60
		};

		AnimationTime();
		AnimationTime(const AnimationTime& inTime);
		AnimationTime& operator=(const AnimationTime& inTime);
		~AnimationTime();

		AnimationTime(const Length inLength);
		explicit AnimationTime(const long long inLength);
		
		AnimationTime operator-() const;
		AnimationTime operator+(const AnimationTime addTime) const; 
		AnimationTime operator+=(const AnimationTime addTime);
		AnimationTime operator-(const AnimationTime subTime) const;
		AnimationTime operator-=(const AnimationTime subTime);
		AnimationTime operator*(const float factor) const;
		AnimationTime operator*(const int factor) const;
		AnimationTime operator*=(const float factor);
		AnimationTime operator*= (const int factor);
		float operator/(const AnimationTime divisor) const;
		AnimationTime operator/(const float divisor) const;
		AnimationTime operator/(const int divisor) const;
		AnimationTime operator/=(const float divisor);
		AnimationTime operator/= (const int divisor);

		friend const AnimationTime operator*(const float factor, const AnimationTime& Time);
		friend const AnimationTime operator*(const int factor, const AnimationTime& Time);

		bool operator == (const AnimationTime& rhs) const;
		bool operator != (const AnimationTime& rhs) const;
		bool operator < (const AnimationTime& rhs) const;
		bool operator > (const AnimationTime& rhs) const;
		bool operator <= (const AnimationTime& rhs) const;
		bool operator >= (const AnimationTime& rhs) const;

	private:
		long long rawTime;
	};

}


#endif
