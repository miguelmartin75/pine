#ifndef __PINE_UTILS_H__
#define __PINE_UTILS_H__

#include "Types.h"

namespace pine
{
	/// \return The user's time in seconds
	inline Seconds GetTimeNow()
	{
		return std::chrono::duration_cast<std::chrono::duration<Seconds, std::ratio<1>>>(Clock::now().time_since_epoch()).count();
	}
}

#endif // __PINE_UTILS_H__