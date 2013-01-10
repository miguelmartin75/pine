///
/// Pine Game Framework
/// Copyright (C) 2012 Anax Creations. All rights reserved.
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

#ifndef __PINE_CONFIG_H__
#define __PINE_CONFIG_H__

#define PINE_YES 1
#define PINE_NO 0

// Set this macro to PINE_YES if you wish the framework to not
// to throw any exceptions
#define PINE_NO_EXCEPTIONS PINE_NO

#define PINE_FLOATING_POINT_DOUBLE 2
#define PINE_FLOATING_POINT_SINGLE 1

/// Change this macro's value if you wish to use single floating
/// point precision (PINE_FLOATING_POINT_SINGLE) or double
/// floating point precision (PINE_FLOATING_POINT_DOUBLE).
/// Default is double.
#define PINE_FLOATING_POINT_PERCISION PINE_FLOATING_POINT_DOUBLE

#endif // __PINE_CONFIG_H__