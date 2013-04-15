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

#ifndef __PINE_ERRORCODE_HPP__
#define __PINE_ERRORCODE_HPP__

#include "config.h"

namespace pine
{
	struct ErrorCode
	{
		enum Code
		{
			/// Indicates that the program exited with no errors
			ExitSuccesful = 0,
			/// Idicates that there was an unknown error that occured
			UnknownError = -1,
			/// Indicates that the engine failed to initialize
			EngineFailedToInitialize = -2
			
			// exception error codes
		#if (PINE_NO_EXCEPTIONS == PINE_NO)
			,
			/// Indicates that a standard exception has occured
			StandardExceptionOccured	= 1,
			/// Indicates that a logical exception has occured
			LogicalExceptionOccured		= 2,
			/// Indicates that a run-time exception has occured
			RuntimeExceptionOccured		= 3,
			/// Indicates that a bad exception has occured
			BadExceptionOccured			= 4
		#endif
		};
	};
}

#endif // __PINE_ERRORCODE_HPP__