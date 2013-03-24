#ifndef __PINE_UTILS_HPP__
#define __PINE_UTILS_HPP__

#include "config.hpp"
#include "types.hpp"

#ifdef USE_BOOST_CHRONO
#	include <boost/chrono.hpp>
#else
#	include <chrono>
#endif // USE_BOOST_CHRONO

namespace pine
{
	/// \return The user's time in seconds
	inline Seconds GetTimeNow()
	{
#	ifdef USE_BOOST_CHRONO
        using namespace boost;
#	else
        using namespace std;
#	endif // USE_BOOST_CHRONO
        
		return chrono::duration_cast<chrono::duration<Seconds, std::ratio<1>>>(chrono::high_resolution_clock::now().time_since_epoch()).count();
	}
}

#endif // __PINE_UTILS_HPP__