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

#ifndef __PINE_STATEDGAME_HPP__
#define __PINE_STATEDGAME_HPP__

#include <pine/Game.hpp>
#include <pine/GameStateStack.hpp>

namespace pine
{
	template <class TGameConcept>
	class StatedGame : public Game<TGameConcept>
	{
	protected:
		
		typedef StatedGame<TGameConcept> Base;
		
	public:
		
		typedef GameState<TGameConcept, TEngineConcept> State;
		typedef GameStateStack<TGameConcept, TEngineConcept> StateStack;
		
		/// \return The GameStack attached to the game
		StateStack& getStateStack() { return _stack; }
		const StateStack& getStateStack() const { return _stack; }
        
		
		void initialize(int argc, char* argv[])
		{
			_stack.setGame(actual_game());
			actual_game().initialize(argc, argv);
		}
		
		void frame_begin() 
		{
            _stack.frame_begin();
			actual_game().begin();
		}

		void update(Seconds deltaTime)
		{
			_stack.update(deltaTime);
            actual_game.update(deltaTime);
		}

		void frame_end()
		{
			_stack.frame_end();
            actual_game.frame_end();
		}
		
	private:
		
		/// The game state stack
		SateStack _stack;
	};
}

#endif // __PINE_GAMELOOP_HPP__
