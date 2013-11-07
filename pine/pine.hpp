///
/// pine
/// Copyright (C) 2013 Miguel Martin (miguel.martin7.5@hotmail.com)
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

#ifndef __PINE_PINE_HPP__
#define __PINE_PINE_HPP__

/// \brief A namespace that contains all classes/functions for the pine library
///
/// This namespace contains all you need to use the pine library.
///
/// \author Miguel Martin
namespace pine
{
}

/// \mainpage
///
/// \section welcome Welcome
///
/// The pine library is a simple, generic game cross-platform, header-only C++ library,
/// designed to make organisation of a game a much simpler process. This library depends
/// heavily on C++ templates, therefore you should be familiar with them before proceeding
/// to learn how to use this library,
///
/// Pine is not made to re-invent the wheel, but rather re-mould the wheel
/// so that it is more easier to use. Pine does not define it's own graphics,
/// physics, sound, etc. library, it merely is made for game management to
/// be a bit easier and simpler. You define your own libraries, or use pre-exsisting ones,
/// this library is like the glue, it helps stick everything together.
///
/// All code that is related to the pine library is within the pine namespace.
///
/// \author Miguel Martin

#define PINE_VERSION_MAJOR 0
#define PINE_VERSION_MINOR 2
#define PINE_PATCH_NUMBER 0

#define PINE_VERSION_NUMBER PINE_VERSION_MAJOR.PINE_VERSION_MINOR.PINE_PATCH_NUMBER

#include <pine/config.hpp>
#include <pine/Game.hpp>
#include <pine/Engine.hpp>

#endif // __PINE_PINE_HPP__