#ifndef __PINE_UTILS_HPP__
#define __PINE_UTILS_HPP__

#include <pine/config.hpp>
#include <pine/types.hpp>

#ifdef PINE_USE_BOOST_CHRONO
#	include <boost/chrono.hpp>
#else
#	include <chrono>
#endif // PINE_USE_BOOST_CHRONO

namespace pine
{
	/// \return The user's time in seconds
	inline Seconds GetTimeNow()
	{
#	ifdef PINE_USE_BOOST_CHRONO
		using namespace boost;
#	else
		using namespace std;
#	endif // PINE_USE_BOOST_CHRONO
		
		return chrono::duration_cast<chrono::duration<Seconds, std::ratio<1> > >(chrono::high_resolution_clock::now().time_since_epoch()).count();
	}
}

#endif // __PINE_UTILS_HPP__