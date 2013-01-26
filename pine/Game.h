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

#ifndef __PINE_GAME_H__
#define __PINE_GAME_H__

#include "GameLoop.h"
#include "GameState.h"
#include "ErrorCode.h"

namespace pine
{
    template <typename TEngine>
	class Game;
	 
	template <typename TGame>
	class GameDelegate
	{
	public:
		
    	typedef TGame Game;
	    friend Game;
		
	private:
		
		
	    virtual void onGameWillInitialize(Game& game, int argc, char* argv[]) = 0;
		
    	virtual void onGameHasInitialized(Game& game, int argc, char* argv[]) = 0;
    };
	
	/// \class Game
	/// \brief Represents a game
	/// \tparam TEngine A class that derives from the Engine interface
	///
	/// This Game class uses the Pine Game Framework as a base,
	/// but lets you decide on the little details (via the Engine template
	/// paramter). The Game class contains a loop, game state stack and
	/// an engine (your engine).
	///
	/// \author Miguel Martin
	template <class TEngine>
	class Game
	{
		typedef Game<TEngine> ThisType;
		
	public:
		
		typedef GameDelegate<ThisType> Delegate;
		typedef GameLoop<ThisType> Loop;
		typedef TEngine Engine;
		
		friend Loop;
		
		Game(int argc, char* argv[], Delegate& delegate, Engine& engine)
			: _errorState(ErrorCode::ExitSuccesful),
			  _isRunning(true),
			  _gameLoop(*this),
			  _engine(engine),
			  _delegate(delegate)
		{
			initialize(argc, argv);
		}
		
		~Game()
		{ _engine.onQuit(_errorState); }
		
		// TODO:
		// update()
		// just incase the client does not have control of the game-loop
		
		
		/// Runs the Game
		/// \return The error code of the Game
		int run()
		{ return _gameLoop.run(); }
		
		/// Exits the Game
		/// \param exitCode
		void exit(int errorCode = ErrorCode::ExitSuccesful)
		{ _errorState = errorCode; _isRunning = false; }
		
		Engine& getEngine()
		{ return _engine; }
		
		Loop& getLoop()
		{ return _gameLoop; }
		
		Delegate& getDelegate()
		{ return _delegate; }
		
	private:
		
		void initialize(int argc, char* argv[])
		{
			// set the engine's game
			_engine.setGame(this);
			
			// notify the game delegate
			getDelegate().onGameWillInitialize(*this, argc, argv);
			
			// initialize the engine for the game
			_engine.initialize(argc, argv);
			
			// notify the game delegate again
			getDelegate().onGameHasInitialized(*this, argc, argv);
		}
		
		
		
		// methods for the game loop
		
		void begin()
		{ _engine.begin(); }
		
		void update(Seconds deltaTime)
		{ _engine.update(deltaTime); }
		
		void end()
		{ _engine.end(); }
		
		void setErrorState(int errorState)
		{ _errorState = errorState; }
		
		int getErrorState() const
		{ return _errorState; }
		
		bool isRunning() const
		{ return _isRunning; }
		
		void doOnFrameRateCalculationUpdated(Seconds framesPerSecond)
		{ _engine.doOnFrameRateCalculationUpdated(framesPerSecond); }
		
        
        /// The delegate of the game
        Delegate& _delegate;
		
		/// The error state of the game
		int _errorState;
		
		/// A flag to represent if the game is running
		bool _isRunning;
		
		/// The Engine of the game
		Engine& _engine;
		
		/// The Game Loop of your game
		Loop _gameLoop;
	};
}

#endif // __PINE_GAMELOOP_H__
