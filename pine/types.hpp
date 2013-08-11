#ifndef __PINE_TYPES_HPP__
#define __PINE_TYPES_HPP__

#include <pine/config.hpp>

namespace pine
{	
	/// The unit used for Frames Per Second
	typedef unsigned int FramesPerSecond;
	
#if (PINE_FLOATING_POINT_PERCISION == PINE_FLOATING_POINT_DOUBLE)
	typedef double Real;
#else
	typedef float Real;
#endif
	
	/// The unit for seconds
	typedef Real Seconds;
}

#endif // __PINE_TYPES_HPP__