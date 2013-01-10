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

#ifndef __PINE_GAMESATE_H__
#define __PINE_GAMESATE_H__

#include "config.h"

namespace pine
{
	template <class TEngine>
	class Game;
	
	template <class TEngine>
	class GameStateStack;
	
	template <class TEngine>
	class GameState
	{
	public:
		
		typedef TEngine Engine;
		typedef Game<TEngine> Game;
		typedef GameStateStack<TEngine> GameStateStack;
		friend GameStateStack;
		
		GameState()
			: _game(nullptr)
		{}
		
		virtual ~GameState() = 0;
		
		/// Used to initialize the GameState
		virtual void initialize() {}
		
		/// Used to load resources for the GameState
		/// \note This is called before initialize()
		virtual void loadResources() {}
		
		/// Used to unload any resources that was loaded in the loadResources function
		virtual void unloadResources() {}
		
		/// Used to update the GameState
		/// \param deltaTime The change in time
		virtual void update(Seconds deltaTime) {}
		
		/// Occurs when you wish to draw the game
		virtual void draw() {}
		
		/// \return The Game attached to the state
		Game& getGame()
		{ return *_game; }
		
		/// \return The Game attached to the state
		const Game& getGame() const
		{ return *_game; }
		
		/// \return The Engine for the Game
		Engine& getEngine()
		{ return _game->getEngine(); }
		
		/// \return The Engine for the Game
		const Engine& getEngine() const
		{ return _game->getEngine(); }
		
		GameStateStack& getGameStateStack()
		{ return getEngine().getGameStateStack(); }
		
		const GameStateStack& getGameStateStack() const
		{ return getEngine().getGameStateStack(); }
		
	private:
		
		/// The game attached to the state
		Game* _game;
	};
	
	template <class TGame>
	GameState<TGame>::~GameState()
	{
		/* do nothing */
	}
}

#endif // __PINE_GAMESATE_H__