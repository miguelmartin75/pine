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

#ifndef PINE_CONFIG_HPP
#define PINE_CONFIG_HPP

#define PINE_FLOATING_POINT_DOUBLE 2
#define PINE_FLOATING_POINT_SINGLE 1

/// Change this macro's value if you wish to use single floating
/// point precision (PINE_FLOATING_POINT_SINGLE) or double
/// floating point precision (PINE_FLOATING_POINT_DOUBLE).
/// Default is double.
#define PINE_FLOATING_POINT_PERCISION PINE_FLOATING_POINT_DOUBLE

/// Uncomment this macro, if you wish to use
/// variadic templates where possible
#define PINE_USE_VARIADIC_TEMPLATES

/// Uncomment this macro, if you wish to use
/// boost::chrono instead of std::chrono
//#define PINE_USE_BOOST_CHRONO

#endif // PINE_CONFIG_HPP
