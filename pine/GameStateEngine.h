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

#ifndef __PINE_GAMESTATEENGINE_H__
#define __PINE_GAMESTATEENGINE_H__

#include "GameEngine.h"
#include "GameState.h"
#include "GameStateStack.h"

namespace pine
{
	/// \brief A GameEngine that uses GameStates
	///
	/// \author Miguel Martin
	template <class TEngine>
	class GameStateEngine
		: public GameEngine<TEngine>
	{
	public:
		
		typedef GameStateEngine<TEngine> Base;
		typedef GameState<TEngine> GameState;
		typedef GameStateStack<TEngine> GameStateStack;
		
		/// Default constructor
		GameStateEngine() {}
		
		/// Destructor
		~GameStateEngine() {}
		
		GameStateEngine(const GameStateEngine& gameStateEngine) = delete;
		GameStateEngine& operator=(const GameStateEngine&) = delete;
		
		/// Resets your Game
		/// \param gameState The GameState you wish to reset the game with
		void resetGame(GameState* gameState)
		{
			_stack.push(gameState, PushType::PushAndPopAllPreviousStates);
		}
		
		/// \return The GameStack attached to the GameStateEngine
		GameStateStack& getGameStateStack()
		{ return _stack; }
		
		/// \return The GameStack attached to the GameStateEngine
		const GameStateStack& getGameStateStack() const
		{ return _stack; }
		
		
		// overridden methods
		
		/// Initializes the GameStateEngine
		/// Required to call in derived class
		/// \note
		/// call this before you initialize your engine
		/// and call finishedInitialization when you're finished initialization
		void initialize(int argc, char* argv[])
		{
			// Set the stack's game reference
			// to our game reference
			_stack.setGame(this->getGame());
			
			GameEngine<TEngine>::initialize(argc, argv);
		}
		
		/// Used for the beginning of a frame
		/// \note
		/// This is required to be call in all derived classes
		void begin()
		{ /* do nothing, let derived class take care of this */ }
		
		/// Updates the GameStateEngine
		/// \param deltaTime The change in time
		/// \note
		/// This is required to be call in all derived classes
		void update(Seconds deltaTime)
		{ _stack.update(deltaTime); }
		
		/// Used for the end of a frame
		/// \note
		/// This is required to be call in all derived classes
		void end()
		{ _stack.draw(); }
		
		/// Shuts the engine down
		void shutDown(int errorCode) { }
		
	protected:
		
		// call this when you finish initializing the engine
		// \note This is REQUIRED to be called
		void finalizeInitialization()
		{
			// finialize initialization
			GameEngine<TEngine>::finalizeInitialization();
		}
		
	private:
		
		/// The stack in the engine
		GameStateStack _stack;
	};
}

#endif // __PINE_GAMESTATEENGINE_H__
