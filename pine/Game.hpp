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

#ifndef __PINE_GAME_HPP__
#define __PINE_GAME_HPP__

#include <cassert>

#include <pine/GameState.hpp>

namespace pine
{
	/// \brief Represents a game
	/// \tparam TEngineConcept A custom engine you wish to use with your game
	///
	/// \author Miguel Martin
	template <class TEngineConcept>
	class Game
	{
	protected:
		
		typedef Game<TEngineConcept> Base;
		
	public:
		
		typedef TEngineConcept Engine;
		
		Game()
			: _isRunning(true),
			  _errorCodeState(0)
		{
		}
		
		/// Quits the Game
		/// \param exitCode The code you wish to exit the game with
		void quit(int errorCode = 0)
		{ _isRunning = false; _errorCodeState = errorCode; }
		
		/// \return The Engine of the game
		Engine& getEngine()
		{ return *_engine; }
		
		const Engine& getEngine() const
		{ return *_engine; }
		
		/// \return The error state of the game
		int getErrorCodeState() const
		{ return _errorCodeState; }
		
		/// \return true if the game is running
		bool isRunning() const
		{ return _isRunning; }
        
		
		
		/********************************************************
		 * In order to add custom functionality, you must
		 * override these methods in a derived class.
		 *******************************************************/
		
		/// Initializes the game
		/// \param engine The engine you wish to initialize the game with
		/// \param argc The number of arguments
		/// \param argv The arguments themself
		/// \note If you override this method, you must call it at the start of the method
        /// \note It is reccomended you override the suggested methods below
        /// \see onWillInitialize
        /// \see onInitialized
		void initialize(Engine& engine, int argc, char* argv[])
		{
			assert(!(engine._game != nullptr) && "Engine already has a game attached to it!");
			
			// set the engine
			_engine = &engine;
			
            // set the engine's game
			getEngine().setGame(this);
			
			// initialize the engine for the game
			getEngine().initialize(argc, argv);
		}
		
		/// begin is called every frame before anything occurs
		/// \note It is reccomended to event handling here.
		/// \note If you override this method, you must call it at the start of the method
		void begin()
		{
			getEngine().begin();
		}
		
        /// Updates the Game
        /// \param deltaTime The change in time
        /// \note
        /// Do not use this for drawing, as the default game loop may
        /// call this method multiple times per frame.
		/// \note If you override this method, you must call it at the start of the method
		void update(Seconds deltaTime)
		{
			getEngine().update(deltaTime);
		}
		
		/// end is called at the end of every frame
		/// \note It is reccomended to do rendering here.
		/// \note If you override this method, you must call it at the start of the method
		void end()
		{
			getEngine().end();
		}
		
	private:
		
		/// The error code state
		int _errorCodeState;
		
		/// The Game Loop of your game
		bool _isRunning;
		
		/// The Engine of the game
		Engine* _engine;
	};
}

#endif // __PINE_GAMELOOP_HPP__
