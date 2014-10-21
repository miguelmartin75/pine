///
/// pine
/// Copyright (C) 2014 Miguel Martin (miguel@miguel-martin.com)
///
///
/// This software is provided 'as-is', without any express or implied warranty.
/// In no event will the authors be held liable for any damages arising from the
/// use of this software.
///
/// Permission is hereby granted, free of charge, to any person
/// obtaining a copy of this software and associated documentation files (the "Software"),
/// to deal in the Software without restriction, including without limitation the rights
/// to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
/// copies of the Software, and to permit persons to whom the Software is
/// furnished to do so, subject to the following conditions:
///
/// 1. The origin of this software must not be misrepresented;
///    you must not claim that you wrote the original software.
///    If you use this software in a product, an acknowledgment
///    in the product documentation would be appreciated but is not required.
///
/// 2. Altered source versions must be plainly marked as such,
///	   and must not be misrepresented as being the original software.
///
/// 3. The above copyright notice and this permission notice shall be included in
///    all copies or substantial portions of the Software.
///

#ifndef PINE_UTILS_HPP
#define PINE_UTILS_HPP

#include <pine/config.hpp>
#include <pine/types.hpp>

#ifdef PINE_USE_BOOST_CHRONO
#	include <boost/chrono.hpp>
#else
#	include <chrono>
#endif // PINE_UTILS_HPP

namespace pine
{
    /// \return The user's time in seconds
    inline Seconds time_now()
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
