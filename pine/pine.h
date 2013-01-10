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

#ifndef __PINE_PINE_H__
#define __PINE_PINE_H__

/// \mainpage
///
/// \section welcome Welcome
///
/// The Pine Game Framework is a simple, generic game cross-platform
/// game-framework. By generic, it is truely generic, if you want to control
/// how you game implements things behind the hood, you can.
/// There is no reason why you can't use your own engine with this library,
/// unless you've already made your engine. Either that or you're just a stubborn
/// dick.
///
/// Pine is not made to re-invent the wheel, but rather re-mould the wheel
/// so that it is more easier to use. Pine does not define it's own graphics,
/// physics, sound, etc libraries*, it merely is made for game management to
/// be a bit easier and simpler.
///
/// All code that is related to the Pine Framework is within the
/// pine namespace.
///
/// \author Miguel Martin

// include headers

#include "config.h"
#include "ErrorCode.h"
#include "GameLoop.h"
#include "Game.h"
#include "Engine.h"
#include "GameStateEngine.h"

#endif // __PINE_PINE_H__