#ifndef TRIG_H
#define TRIG_H

namespace EngineSpace
{
	class Trig
	{
	public:
		static float cos(float in);
		static float sin(float in);
		static float tan(float in);
		static float atan(float in);
		static float atan2(float y, float x);
		static float acos(float in);
		static float asin(float in);
		static void cossin(float& y, float& z, float x);
		static float sqrt(float in);
		static float rsqrt(float in);
		static float abs(float in);
	};
}

#endif
