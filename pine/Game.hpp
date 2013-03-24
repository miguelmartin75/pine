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

#ifndef __PINE_GAME_HPP__
#define __PINE_GAME_HPP__

#include "GameLoop.hpp"
#include "GameState.hpp"
#include "ErrorCode.hpp"

namespace pine
{
	template <typename TEngineConcept>
	class Game;
    
	/// \brief Handles game specific
	/// \tparam TGame A Game class
	/// \author Miguel Martin
	template <typename TGame>
	class GameHandler
	{
	public:
		
		typedef TGame Game;
		friend Game;
		
		/// Destructor
		virtual ~GameHandler() = 0;
		
	private:
		
		/// Occurs when the Game will initialize
		/// \param game The Game that will initialize
		/// \param argc The amount of arguments in the command line
		/// \param argv The arguments in the command line
		/// \note
		/// This is where you do pre-initialization,
		/// e.g. setting window/context cofiguration
		virtual void onGameWillInitialize(Game& game, int argc, char* argv[]) {}
		
		/// Occurs when the Game has initialization
		/// \param game The Game that has initialized
		/// \param argc The amount of arguments in the command line
		/// \param argv The arguments in the command line
		/// \note
		/// This is for post-initialization,
		/// e.g. Specific initialization that can only be done after
		/// the game is initalized
		virtual void onGameHasInitialized(Game& game, int argc, char* argv[]) {}
	};
	
	template <typename TGame>
	GameHandler<TGame>::~GameHandler()
	{
	}
	
	/// \brief Represents a game
	/// \tparam TEngineConcept An Engine concept, which derives from the Engine class
	///
	/// This Game class uses the Pine Game Framework as a base,
	/// but lets you decide on the little details (via the Engine template
	/// paramter). The Game class contains a loop, and
	/// an engine (your engine).
	///
	/// \author Miguel Martin
	template <class TEngineConcept>
	class Game
	{
		typedef Game<TEngineConcept> ThisType;
		
	public:
		
		typedef GameHandler<ThisType> Handler;
		typedef GameLoop<ThisType> Loop;
		typedef TEngineConcept Engine;
		
		friend Loop;
		
		Game(int argc, char* argv[], Handler& handler, Engine& engine)
			:_gameLoop(*this),
			  _engine(engine),
			  _handler(handler)
		{
			initialize(argc, argv);
		}
		
		~Game()
		{
			// shut down our engine
			_engine.shutDown(getErrorCodeState());
		}
		
		/// Updates the game loop
		void update()
		{ _gameLoop.update(); }
		
		/// Runs the Game
		/// \return The error code of the Game
		int run()
		{ return _gameLoop.run(); }
		
		/// Exits the Game
		/// \param exitCode The code you wish to exit the game with
		void exit(int errorCode = ErrorCode::ExitSuccesful)
		{ _gameLoop.exit(errorCode); }
		
		/// \return The Engine of the game
		Engine& getEngine()
		{ return _engine; }
		
		/// \return The Game Loop that controls the game
		Loop& getLoop()
		{ return _gameLoop; }
		
		/// \return The Game handler
		Handler& getHandler()
		{ return _handler; }
		
		/// \return The error state of the game
		int getErrorCodeState() const
		{ return _gameLoop.getErrorCodeState(); }
		
		/// \return true if the game is running
		bool isRunning() const
		{ return _gameLoop.isRunning(); }
        
	private:
		
		// initializes the game
		void initialize(int argc, char* argv[])
		{
			// set the engine's game
			_engine.setGame(this);
			
			// notify the game delegate
			getHandler().onGameWillInitialize(*this, argc, argv);
			
			// initialize the engine for the game
			_engine.initialize(argc, argv);
			
			// notify the game delegate again
			getHandler().onGameHasInitialized(*this, argc, argv);
		}
		
		
		
		// methods for the game loop
		
		void begin()
		{ _engine.begin(); }
		
		void update(Seconds deltaTime)
		{ _engine.update(deltaTime); }
		
		void end()
		{ _engine.end(); }
		
		
		
		/// The delegate of the game
		Handler& _handler;
		
		/// The Engine of the game
		Engine& _engine;
		
		/// The Game Loop of your game
		Loop _gameLoop;
	};
}

#endif // __PINE_GAMELOOP_HPP__
