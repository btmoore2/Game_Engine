#ifndef QUAT_APP_H
#define QUAT_APP_H

#include "Quat.h"

namespace EngineSpace
{
	class QuatApp
	{
	public:
		static void Slerp(Quat& result, const Quat& source, const Quat& target, const float slerpFactor);

		static void SlerpArray(Quat* result, const Quat* source, const Quat* target, const float slerpFactor, const int numQuats);
	};
}

#endif 