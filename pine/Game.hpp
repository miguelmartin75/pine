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
	///
	/// \author Miguel Martin
	template <class TGame>
	class Game
	{
	public:

        typedef TGame actual_game_type;

    protected:

        actual_game_type& actual_game() { return *static_cast<this_type*>(this); }
        const actual_game_type& actual_game() const { return *static_cast<const this_type*>(this); }

	public:
		
		Game() : 
            _isRunning(true),
			_errorCodeState(0)
		{
		}
        
		/// Quits the Game
		/// \param exitCode The code you wish to exit the game with
		void quit(int errorCode = 0)
		{ _running = false; _error_state = errorCode; }
		
		/// \return The error state of the game
		int error_state() const { return _error_state; }
		
		/// \return true if the game is running
		bool running() const { return _isRunning; }
		
		/// begin is called every frame before anything occurs
		/// \note It is reccomended to event handling here.
		/// \note If you override this method, you must call it at the start of the method
		void frame_start()
		{
            actual_actual().frame_start();
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
		/// \note It is recommended to do rendering here.
		/// \note If you override this method, you must call it at the start of the method
		void frame_end()
		{
            actual_game().frame_end();
		}
		
	private:
		
		/// The current error code state
		int _error_state;
		
		/// The Game Loop of your game
		bool _running;
	};
}

#endif // __PINE_GAMELOOP_HPP__
