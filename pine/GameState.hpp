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

#ifndef PINE_GAME_SATE_HPP
#define PINE_GAME_SATE_HPP

#include <pine/types.hpp>

namespace pine
{
	template <class TEngineConcept>
	class Game;
	
	template <class TGameConcept, class TEngineConcept>
	class GameStateStack;
	
	/// \brief Describes a state in your game
	/// \tparam TGameConcept A game concept
	/// \tparam TEngineConcept An engine concept, which derives from GameEngine
	///
	/// A game state is simply a state in your game,
	/// this could be a main menu, the game itself,
	/// the pause menu, or anything you want.
	/// All GameStates have a reference to your engine and game.
	///
	/// \author Miguel Martin
	template <class TGame>
	class GameState
	{
	public:
		
		typedef TGame Game;
		typedef GameStateStack<TGameConcept> GameStateStack;

		friend GameStateStack;
		
		/// Default constructor
		GameState() : 
            _game(nullptr)
		{
		}
		
		/// Destructor
		virtual ~GameState() = 0;
		
		
		/// \return The Game attached to the state
		Game& getGame()
		{ return *_game; }
		
		/// \return The Game attached to the state
		const Game& getGame() const
		{ return *_game; }
		
	private:
		
		/// Used to initialize the GameState
		virtual void initialize() {}
		
		/// Used to load resources for the GameState
		/// \note This is called before initialize()
		virtual void loadResources() {}
		
		/// Used to unload any resources that was loaded in the loadResources function
		virtual void unloadResources() {}

        virtual void frame_begin() { }
		
		virtual void update(Seconds deltaTime) {}
		
		virtual void frame_end() {}
		
	private:
		
		/// The game attached to the state
		Game* _game; // guaranteed to not be null
	};
	
	template <class TGame>
	GameState<TGame>::~GameState()
	{
		/* do nothing */
	}
}

#endif // PINE_GAME_STATE_HPP
