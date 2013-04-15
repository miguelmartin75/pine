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

#ifndef __PINE_GAMELOOP_HPP__
#define __PINE_GAMELOOP_HPP__

#include <cassert>
#include <vector>
#include <algorithm>

#include "config.hpp"
#include "types.hpp"
#include "utils.hpp"
#include "ErrorCode.hpp"

#if (PINE_NO_EXCEPTIONS == PINE_NO)
#	include <stdexcept>
#endif


namespace pine
{
	/// \brief Defines a GameLoop
	///
	/// A class that represents a game loop
	///
	/// \tparam TGameConcept A game concept is a class that must have these methods implemented:
	///                     - void begin()					- occurs at the beginning of a frame
	///                     - void update(double deltaTime) - updates the game
	///                     - void end()					- occurs at the end of a frame
	///
	/// \author Miguel Martin
	template <class TGameConcept>
	class GameLoop
	{
	public:
		
		/// The default max frame time
		static const Real DEFAULT_MAX_FRAME_TIME;
		
		/// Constructs a game loop attached to a game
		/// \param game The Game you wish to attach
		/// \param maxFrameTime The maximum time for a frame to be completed
		GameLoop(TGameConcept& game, Real maxFrameTime = DEFAULT_MAX_FRAME_TIME)
			: _isRunning(true),
		      _errorCodeState(0),
			  _game(game),
			  _deltaTime(0),
			  _simulationTime(0),
			  _maxFrameTime(maxFrameTime),
			  _isInitialized(false)
		{
			setSimulationFps(100); // 100 by default
		}
		
		/// Constructs a game loop attached to a game
		/// \param game The Game you wish to attach
		/// \param simulationFps The frames per second you wish to simulate the game at
		/// \param fpsCalculationPeriod The time to re-calculate the runtime FPS
		/// \param maxFrameTime The maximum time for a frame to be completed
		GameLoop(TGameConcept& game, FramesPerSecond simulationFps, Real maxFrameTime = DEFAULT_MAX_FRAME_TIME)
			: _isRunning(true),
			  _errorCodeState(0),
			  _game(game),
			  _deltaTime(0),
			  _simulationTime(0),
              _maxFrameTime(maxFrameTime),
			  _isInitialized(false)
		{
			setSimulationFps(simulationFps);
		}
		
		~GameLoop()
		{
		}
		
		
		
		/// Initializes the GameLoop
		/// \note You only require to call this method if you use update()
		void initialize()
		{
			_startTime = GetTimeNow();
			_currentTime = GetTimeNow();
			_isInitialized = true;
		}
		
		/// Updates the game loop
		/// \note This will not check for exceptions
		void update()
		{
			assert(isInitialized() && "GameLoop is not initialized");
			
			// This loop is CLOSELY based off of this article:
			// http://gafferongames.com/game-physics/fix-your-timestep/
			// I am not sure if this will work with a physics engine entirely (e.g. Box2D)
			// it should, but I have not tested this.
            
			// the beginning of the frame
			_game.begin();
			
			Seconds newTime = GetTimeNow();
			Seconds frameTime = newTime - _currentTime;
			_currentTime = newTime;
			
			// cap the loop delta time
			if(frameTime >= getMaxFrameTime())
			{
				frameTime = getMaxFrameTime();
			}
			
			_accumulator += frameTime;
			
			
			// Update our game
			while(_accumulator >= getDeltaTime())
			{
				_game.update(getDeltaTime()); // update the game (with the constant delta time)
				_accumulator -= getDeltaTime(); // decrease the accumulator
				_simulationTime += getDeltaTime(); // increment the simulation time
			}
			
			// the end of the frame
			_game.end();
			
			++_frame;
			++_amountOfFramesSinceStart;
		}
		
		/// Runs the game loop
		/// \return Any error codes that may occur
		int run()
		{
#if (PINE_NO_EXCEPTIONS == PINE_NO)
			try
			{
#endif // (PINE_NO_EXCEPTIONS == PINE_NO)
				
				initialize();
				
                while (isRunning())
                {
                    update();
                }
				
#if (PINE_NO_EXCEPTIONS == PINE_NO)
			}
			catch(std::runtime_error& e)
			{
				_errorCodeState = (ErrorCode::RuntimeExceptionOccured);
			}
			catch(std::logic_error& e)
			{
				_errorCodeState = (ErrorCode::LogicalExceptionOccured);
			}
			catch(std::bad_exception& e)
			{
				_errorCodeState = (ErrorCode::BadExceptionOccured);
			}
			catch(std::exception& e)
			{
				_errorCodeState = (ErrorCode::StandardExceptionOccured);
			}
#endif // (PINE_NO_EXCEPTIONS == PINE_NO)
			
			return getErrorCodeState();
		}
		
		/// Exits the GameLoop
		/// \param errorCode The error code you wish to exit with
		void exit(int errorCode = 0)
		{
			_isRunning = false;
			_errorCodeState = errorCode;
		}
		
		/// Sets the frames per second that the game is simulated at
		/// \param fps The FPS you wish the game to be updated at
		void setSimulationFps(FramesPerSecond fps)
		{
			_simulationFps = fps;
			_deltaTime = 1.0 / getSimulationFps();
		}
		
		/// \return The frames per second that the game is simulated at
		FramesPerSecond getSimulationFps() const
		{ return _simulationFps; }
		
		/// \return The total simulation time for the game
		Seconds getSimulationTime()
		{ return _simulationTime; }
		
		/// \return The amount of frames per second the game is currently running at
		Real getRuntimeFps() const
		{
			Real temp = _frame / (GetTimeNow() - _startTime);
			_frame = _startTime = 0; // reset the frame and start time
			return temp;
		}
		
		/// \return The change in time
		/// \note This delta time is actually constant depending on your FPS
		Real getDeltaTime() const
		{ return _deltaTime; }
		
		/// \return (not sure, should look this up lol, but it's accumlator/dt)
		/// \note should be called in your game's end function (i.e. when you are drawing)
		Real getAlpha() const
		{ return _accumulator / getDeltaTime(); }
		
		/// \return The amount of the games since the loop started
		size_t getAmountOfFramesSinceStart() const
		{ return _amountOfFramesSinceStart; }
		
		/// \return Maximum time a frame has to complete a loop, so it doesn't clog up the game loop
		Real getMaxFrameTime() const
		{ return _maxFrameTime; }
		
		/// \return true if the GameLoop is running
	    bool isRunning() const
		{ return _isRunning; }
		
		/// \return The current error code
		int getErrorCodeState() const
		{ return _errorCodeState; }
		
		/// \return true if the GameLoop is initialized
		bool isInitialized() const
		{ return _isInitialized; }
		
	private:
        
		/// Determines if the loop is initialized
		bool _isInitialized;
		
		/// Determines if the loop is running
		bool _isRunning;
		
		/// Holds an error code
		int _errorCodeState;
		
		/// Maximum time a frame has to complete a loop, so it doesn't clog up the game loop.
		/// This is stored as a frequency, i.e. 1 / MIN_FPS
		const Real _maxFrameTime;
		
		/// Total simulation time
		Seconds _simulationTime;
		
		/// Time that the game loop started (since initialize was called)
		Seconds _startTime;
		
		/// Holds the current time
		Seconds _currentTime;
		
		/// The rate the simulation of the game will be updated by
		FramesPerSecond _simulationFps;
		
		/// The delta time (this is constant, due to the type of loop this is)
		Real _deltaTime;
		
		/// Contains the frame number for calculation of fps
		size_t _frame;
		
		/// The amount of the games since the loop started
		size_t _amountOfFramesSinceStart;
		
		/// Used to acculate time in the loop
		Real _accumulator;
		
		/// The game tied to the game loop
		TGameConcept& _game;
	};
    
    template <class Game>
    const Real GameLoop<Game>::DEFAULT_MAX_FRAME_TIME = 1 / 4.0f;
}

#endif // __PINE_GAMELOOP_HPP__