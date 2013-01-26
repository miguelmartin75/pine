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

#ifndef __PINE_GAMEENGINE_H__
#define __PINE_GAMEENGINE_H__

#include "Game.h"

namespace pine
{
	/// \brief A base class
	template <class TEngine>
	class GameEngine
	{
	public:
		
		friend Game<TEngine>;
		typedef Game<TEngine> Game;
		typedef GameEngine<TEngine> Base;
		
		GameEngine()
			: _game(nullptr),
			  _isInitialized(false)
		{
		}
		
		~GameEngine() {}
		
		Game& getGame()
		{ return *_game; }
		
		const Game& getGame() const
		{ return _game; }
		
		
		
		/********************************************************
		 * In order to add custom functionality, you must
		 * override these methods in a derived class.
		 *******************************************************/
		
		void initialize(int argc, char* argv[]) {}
		
		void begin() {}
		
		void update(Seconds deltaTime) {}
		
		void end() {}
		
		void onQuit(int errorCode) {}
		
		void doOnFrameRateCalculationUpdated(Seconds framesPerSecond) {}
		
		bool isInitialized() const
		{ return _isInitialized; }
		
	protected:
		
		// call this when you're done initializing
        // \note You are REQUIRED to call this method
		void finalizeInitialization()
		{
			_isInitialized = true;
		}
		
	private:
		
		// called by the Game class
		void setGame(Game* game)
		{ _game = game; }
		
        /// A reference the Game the Engine is attached to
		Game* _game;

	    /// Determines if the engine has been initialized	
		bool _isInitialized;
	};
}

#endif // __PINE_GAMEENGINE_H__
