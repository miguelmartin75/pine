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

#include "Engine.h"
#include "GameState.h"
#include "GameStateStack.h"

namespace pine
{
	template <class TEngine>
	class GameStateEngineDelegate
	{
		friend TEngine;
		
	public:
		
		typedef GameStateEngineDelegate<TEngine> Base;
		typedef TEngine Engine;
		typedef typename Engine::GameState GameState;
		
		virtual ~GameStateEngineDelegate() {}
		
		/// \return The default game state of your game
		virtual GameState* getDefaultGameState() = 0;
		
		/// Used to initialize the game with command line arguments
		/// \param sender The engine that sent this notification
		/// \param argc The amount of arguments in the command line
		/// \param argv The arguments themself
		/// \example
		/// You could use the command line to start you game at level 3 on difficulty hardcore,
		/// or something along those lines.
		/// e.g. This is what the command line arguments may look like:
		/// myGame difficulty=hardcore level=3
		virtual bool initialize(Engine& sender, int argc, char* argv[]) = 0;
	};
	
	template <class TEngine>
	class GameStateEngine
		: public Engine<TEngine>
	{
	public:
		
		typedef GameStateEngine<TEngine> Base;
		typedef GameStateEngineDelegate<GameStateEngine<TEngine> > Delegate;
		typedef GameState<TEngine> GameState;
		typedef GameStateStack<TEngine> GameStateStack;
		
		GameStateEngine(Delegate& delegate)
				: _delegate(delegate)
		{
		}
		
		GameStateEngine(const GameStateEngine& gameStateEngine) = delete;
		GameStateEngine& operator=(const GameStateEngine&) = delete;
		
		virtual ~GameStateEngine() = 0;
		
		/// Resets your Game
		void reset()
		{
			_stack.push(_delegate.getDefaultGameState(), PushType::PushAndPopAllPreviousStates);
		}
		
		
		
		// overridden methods
		
		// required to call in derived classes
		virtual bool initialize(int argc, char* argv[]) override
		{
			_stack.setGame(this->getGame());
			
			if(!getDelegate().initialize(*this, argc, argv))
				return false;

			reset();
			return true;
		}
		
		virtual void begin() override
		{ /* do nothing, let derived class take care of this */ }
		
		// required to call in derived classes
		virtual void update(Seconds deltaTime) override
		{ _stack.update(deltaTime); }
		
		// required to call in derived classess
		virtual void end() override
		{ _stack.draw(); }
		
		virtual void onQuit(int errorCode) override
		{ /* do nothing */ }
		
		// notification method when framerate is recalculated
		// (not required)
		virtual void doOnFrameRateCalculationUpdated(Seconds framesPerSecond) override
		{}
		
		GameStateStack& getGameStateStack()
		{ return _stack; }
		
		const GameStateStack& getGameStateStack() const
		{ return _stack; }
		
	private:
		
		Delegate& getDelegate()
		{ return _delegate; }
		
		const Delegate& getDelegate() const
		{ return _delegate; }
		
		Delegate& _delegate;
		
		GameStateStack _stack;
	};
	
	template <class TEngine>
	GameStateEngine<TEngine>::~GameStateEngine()
	{
	}
}

#endif // __PINE_GAMESTATEENGINE_H__