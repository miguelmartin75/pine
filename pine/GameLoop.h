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

#ifndef __PINE_GAMELOOP_H__
#define __PINE_GAMELOOP_H__

#include "config.h"
#include "Types.h"
#include "Utils.h"
#include "ErrorCode.h"

namespace pine
{
	/// \class Game
	/// \brief Defines a GameLoop
	///
	/// A class that represents a game loop
	///
	/// \tparam Game A class that must have these methods implemented:
	///				- void begin()					- occurs at the beginning of a frame
	///			    - void update(double deltaTime) - updates the game
	///				- void end()					- occurs at the end of a frame
	///				- int getErrorState()			- gets the error state of the game
	///				- bool isRunning()				- determines whether the game is still running
	///
	/// \tparam Real The precision you want when working with floating points,
	///				 you may provide any type, but it is suggested you choose either float or double.
	///				 The Real type is used for calculation of the actual frames per second of a loop and
	///				 storage for seconds.
	/// \author Miguel Martin
	template <class Game>
	class GameLoop
	{
	public:
		
		/// Constructs a game loop attached to a game
		/// \param game The Game you wish to attach
		GameLoop(Game& game)
			: _game(game),
			  _runtimeFps(0),
			  _deltaTime(0),
			  _simulationTime(0),
			  _fpsCalculationPeriod(1) // 1 second by default
		{
			setSimulationFps(100); // 100 by default
		}
		
		/// Constructs a game loop attached to a game
		/// \param game The Game you wish to attach
		/// \param simulationFps The frames per second you wish to simulate the game at
		/// \param fpsCalculationPeriod The time to re-calculate the runtime FPS
		GameLoop(Game& game, FramesPerSecond simulationFps, Seconds fpsCalculationPeriod = 1)
			: _game(game),
			  _runtimeFps(0),
			  _deltaTime(0),
			  _simulationTime(0),
			  _fpsCalculationPeriod(fpsCalculationPeriod) // 1 second by default
		{
			setSimulationFps(simulationFps);
		}
		
		
		
		/// Runs the game loop
		/// \return Any error codes that may occur
		int run()
		{
			// This loop is CLOSELY based off of this article:
			// http://gafferongames.com/game-physics/fix-your-timestep/
			// I am not sure if this will work with a physics engine entirely (e.g. Box2D)
			// it should, but I have not tested this.
			
#if (PINE_NO_EXCEPTIONS == PINE_NO)
			try
			{
#endif // AC_GAMELOOP_HANDLE_EXCEPTIONS
				
				const Real minDeltaTime = 1 / 4.0f; // minimum delta time
				
				_game.doOnFrameRateCalculationUpdated(0);
				
				Seconds newTime = 0;		// declared outside loop for optimization reasons (although compiler will probably do this)
				Seconds loopDeltaTime = 0;	// declared outside loop for optimization reasons (although compiler will probably do this)
				Seconds start = GetTimeNow(); // holds the starting time
				Seconds updateTime = GetTimeNow(); // used to update time
				Seconds currentTime = GetTimeNow(); // holds the current time
				for(size_t frame = 0; _game.isRunning(); ++frame)
				{
					// the beginning of the frame
					_game.begin();
					
					newTime = GetTimeNow();
					loopDeltaTime = newTime - currentTime;
					currentTime = newTime;
					
					// cap the loop delta time
					if(loopDeltaTime >= minDeltaTime)
					{
						loopDeltaTime = minDeltaTime; // note: max frame time to avoid spiral of death
					}
					
					_accumulator += loopDeltaTime;
					
					
					// Update our game
					while(_accumulator >= getDeltaTime())
					{
						_game.update(getDeltaTime()); // update the game (with the constant delta time)
						_accumulator -= getDeltaTime(); // decrease the accumulator
						_simulationTime += getDeltaTime(); // increment the simulation time
					}
					
					// the end of the frame
					_game.end();
					
					// calculate the FPS
					if((GetTimeNow() - updateTime) >= getFpsCalculationPeriod())
					{
						_runtimeFps = frame / (GetTimeNow() - start);
						
						updateTime = GetTimeNow();
						
						_game.doOnFrameRateCalculationUpdated(getRuntimeFps());
					}
				}
#if (PINE_NO_EXCEPTIONS == PINE_NO)
			}
			catch(std::runtime_error& e)
			{
				_game.setErrorState(ErrorCode::RuntimeExceptionOccured);
			}
			catch(std::logic_error& e)
			{
				_game.setErrorState(ErrorCode::LogicalExceptionOccured);
			}
			catch(std::bad_exception& e)
			{
				_game.setErrorState(ErrorCode::BadExceptionOccured);
			}
			catch(std::exception& e)
			{
				_game.setErrorState(ErrorCode::StandardExceptionOccured);
			}
#endif // AC_GAMELOOP_HANDLE_EXCEPTIONS
			
			return _game.getErrorState();
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
		
		/// \return The amount of frames per second the game is running at
		/// \note This is pre-calculated every x seconds
		/// \see setTimeToCalculateFps in order to modify the accuracy of this function
		/// \see getTimeToCalculateFps in order see the accuracy of this function
		Real getRuntimeFps() const
		{ return _runtimeFps; }
		
		/// Sets the time amount to calculate the frames per second of the actual game loop
		/// \param seconds The amount of seconds you wish to re-calculate the frame rate
		/// \note This is not simulated frame rate, but the actual frame rate of the loop itself
		void setFpsCalculationPeriod(Seconds seconds)
		{ _fpsCalculationPeriod = seconds; }
		
		/// \return The time, in seconds, that the game loop will re-calculate the frame rate for the actual game loop
		Seconds getFpsCalculationPeriod() const
		{ return _fpsCalculationPeriod; }
		
		/// \return The change in time
		/// \note This delta time is actually constant depending on your FPS
		Real getDeltaTime() const
		{ return _deltaTime; }
		
		/// \return (not sure, should look this up lol, but it's accumlator/dt)
		/// \note should be called in your game's end function (i.e. when you are drawing)
		Real getAlpha() const
		{ return _accumulator / getDeltaTime(); }
		
	private:
		
		/// The game tied to the game loop
		Game& _game;
		
		/// The run-time frame rate of the game
		Real _runtimeFps;
		
		/// The delta time (this is constant)
		Real _deltaTime;
		
		/// Total simulation time
		Seconds _simulationTime;
		
		/// The amount of time before re-calculation of FPS occurs
		Seconds _fpsCalculationPeriod;
		
		/// The rate the simulation of the game will be updated by
		FramesPerSecond _simulationFps;
		
		/// used to acculate time in the loop
		Real _accumulator;
	};
}

#endif // __PINE_GAMELOOP_H__