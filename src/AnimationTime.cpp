#include "AnimationTime.h"

namespace EngineSpace
{
	AnimationTime::AnimationTime()
		:rawTime(0)
	{

	}

	AnimationTime::AnimationTime(const AnimationTime& inTime)
		:rawTime(inTime.rawTime)
	{

	}

	AnimationTime& AnimationTime::operator= (const AnimationTime& inTime)
	{
		this->rawTime = inTime.rawTime;
		return *this;
	}

	AnimationTime::~AnimationTime()
	{

	}

	AnimationTime::AnimationTime(const Length inLength)
		:rawTime(0)
	{
		switch (inLength)
		{
		case AnimationTime::Length::ZERO :
			break;
		case AnimationTime::Length::MILLISECOND :
			this->rawTime = 30000;
			break;
		case AnimationTime::Length::SECOND :
			this->rawTime = 30000000;
			break;
		case AnimationTime::Length::MINUTE :
			this->rawTime = 1800000000;
			break;
		case AnimationTime::Length::FPS30 :
			this->rawTime = 1000000;
			break;
		case AnimationTime::Length::FPS60 :
			this->rawTime = 500000;
			break;
		}
	}

	AnimationTime::AnimationTime(const long long inLength)
		:rawTime(inLength)
	{

	}

	AnimationTime AnimationTime::operator-() const
	{
		return AnimationTime(-this->rawTime);
	}

	AnimationTime AnimationTime::operator+(const AnimationTime addTime) const
	{
		return AnimationTime(this->rawTime + addTime.rawTime);
	}

	AnimationTime AnimationTime::operator+=(const AnimationTime addTime)
	{
		this->rawTime += addTime.rawTime;
		return *this;
	}
	AnimationTime AnimationTime::operator-(const AnimationTime subTime) const
	{
		return AnimationTime(this->rawTime - subTime.rawTime);
	}

	AnimationTime AnimationTime::operator-=(const AnimationTime subTime)
	{
		this->rawTime -= subTime.rawTime;
		return *this;
	}

	AnimationTime AnimationTime::operator*(const float factor) const
	{
		return AnimationTime(this->rawTime * (long long)factor);
	}

	AnimationTime AnimationTime::operator*(const int factor) const
	{
		return AnimationTime(this->rawTime * (long long)factor);
	}

	AnimationTime AnimationTime::operator*=(const float factor)
	{
		this->rawTime *= (long long)factor;
		return *this;
	}

	AnimationTime AnimationTime::operator*= (const int factor)
	{
		this->rawTime *= (long long)factor;
		return *this;
	}

	float AnimationTime::operator/(const AnimationTime divisor) const
	{

		return((float)this->rawTime / (float)divisor.rawTime);
	}

	AnimationTime AnimationTime::operator/(const float divisor) const
	{
		return AnimationTime(this->rawTime / (long long)divisor);
	}

	AnimationTime AnimationTime::operator/(const int divisor) const
	{
		return AnimationTime(this->rawTime / (long long)divisor);
	}

	AnimationTime AnimationTime::operator/=(const float divisor)
	{
		this->rawTime /= (long long)divisor;
		return *this;
	}

	AnimationTime AnimationTime::operator/= (const int divisor)
	{
		this->rawTime /= (long long)divisor;
		return *this;
	}

	const AnimationTime operator*(const float factor, const AnimationTime& Time)
	{
		float convert = factor * (float)Time.rawTime;
		return AnimationTime((long long)convert);
	}

	const AnimationTime operator*(const int factor, const AnimationTime& Time)
	{
		return AnimationTime((long long)factor * Time.rawTime);
	}

	bool AnimationTime::operator == (const AnimationTime& rhs) const
	{
		return( this->rawTime == rhs.rawTime);
	}
	bool AnimationTime::operator != (const AnimationTime& rhs) const
	{
		return(this->rawTime != rhs.rawTime);
	}

	bool AnimationTime::operator < (const AnimationTime& rhs) const
	{
		return(this->rawTime < rhs.rawTime);
	}
	bool AnimationTime::operator > (const AnimationTime& rhs) const
	{
		return(this->rawTime > rhs.rawTime);
	}

	bool AnimationTime::operator <= (const AnimationTime& rhs) const
	{
		return(this->rawTime <= rhs.rawTime);
	}

	bool AnimationTime::operator >= (const AnimationTime& rhs) const
	{
		return(this->rawTime >= rhs.rawTime);
	}
}