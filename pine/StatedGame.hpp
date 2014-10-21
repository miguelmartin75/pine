///
/// pine
/// Copyright (C) 2014 Miguel Martin (miguel@miguel-martin.com)
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

#ifndef PINE_STATED_GAME_HPP
#define PINE_STATED_GAME_HPP

#include <pine/Game.hpp>

namespace pine
{
	template <class TGame, class TEngine = void>
	struct StatedGame : Game<TGame, TEngine>
	{
	protected:
		
		typedef StatedGame<TGame> Base;

    private:
        
        TGame& actual_game() { return *static_cast<TGame*>(this); }
        const TGame& actual_game() const { return *static_cast<const TGame*>(this); }
		
	public:
        using State = GameState<TGame>;
        using StateStack = GameStateStack<TGame>;
        
        /// \return The GameStack attached to the game
        StateStack& getStateStack() { return _stack; }
        const StateStack& getStateStack() const { return _stack; }

		
		void initialize(int argc, char* argv[])
		{
            _stack.initialize(argc, argv);
			actual_game().initialize(argc, argv);
		}
		
		void frame_begin() 
		{
			actual_game().begin();
		}

		void update(Seconds deltaTime)
		{
            actual_game.update(deltaTime);
		}

		void frame_end()
		{
            WithStates<Game>::frame_end();
            actual_game.frame_end();
		}
        
    private:

        StateStack _stack;
	};
}

#endif // PINE_STATED_GAME_HPP
