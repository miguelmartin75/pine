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
	template <class TEngineConcept, class TGameConcept>
	class StatedGame;
	
	/// \brief Represents a game
	/// \tparam TEngineConcept The engine you wish your game to use
	/// \tparam TGameConcept A parameter used only if you are sub-classing this class
	///
	/// This class is a basic template used for a stated game, that is, a game
	/// that uses game states. It is not necessery to override this class's
	/// methods if you do not need to; unless you want to do something that this class
	/// does not do and that your basic engine does not provide.
	///
	/// \see DefaultStatedGame If you do not wish to override anything
	/// \see GameState For more details about a game state
	/// \see GameStateStack For more details about a game state stack
	///
	/// \author Miguel Martin
	template <class TEngineConcept, class TGameConcept>
	class StatedGame : public Game<TEngineConcept>
	{
	protected:
		
		typedef StatedGame<TEngineConcept, TGameConcept> Base;
		
	public:
		
		typedef GameState<TGameConcept, TEngineConcept> State;
		typedef GameStateStack<TGameConcept, TEngineConcept> GameStateStack;
		
		/// \return The GameStack attached to the game
		GameStateStack& getGameStateStack()
		{
			return _stack;
		}
		
		/// \return The GameStack attached to the game
		const GameStateStack& getGameStateStack() const
		{
			return _stack;
		}
        
		
		
		/********************************************************
		 * In order to add custom functionality, you must
		 * override these methods in a derived class.
		 *******************************************************/
		
		/// Initializes the game
		/// \param engine The engine you wish to initialize the game with
		/// \param argc The number of arguments
		/// \param argv The arguments themself
		/// \note If you override this method, you must call it at the start of the method
		void initialize(TEngineConcept& engine, int argc, char* argv[])
		{
			Game<TEngineConcept>::initialize(engine, argc, argv);
			
			// Set the stack's game reference
			// to our game reference
			_stack.setGame(*static_cast<TGameConcept*>(this));
		}
		
		/// begin is called every frame before anything occurs
		/// \note It is reccomended to event handling here.
		/// \note If you override this method, you must call it at the start of the method
		void begin()
		{
			Game<TEngineConcept>::begin();
		}
		
        /// Updates the Game
        /// \param deltaTime The change in time
        /// \note
        /// Do not use this for drawing, as the default game loop may
        /// call this method multiple times per frame.
		/// \note If you override this method, you must call it at the start of the method
		void update(Seconds deltaTime)
		{
			Game<TEngineConcept>::update(deltaTime);
			
			// Update the stack
			_stack.update(deltaTime);
		}
		
		/// end is called at the end of every frame
		/// \note It is reccomended to do rendering here.
		/// \note If you override this method, you must call it at the start of the method
		void end()
		{
			Game<TEngineConcept>::end();
			
			// Call draw on the stack
			_stack.draw();
		}
		
	private:
		
		/// The game state stack
		GameStateStack _stack;
	};
	
	
	/// \brief The default implementation of the StatedGame
	///
	/// This is a useful class if you are too lazy to not override any of the methods present in
	/// the StateGame class. You can just use it up and running for your game.
	///
	/// \example
	/// typedef StatedGame<MyEngine> PongGame;
	/// PongGame pong(argc, argv, engine);
	/// // etc.
	///
	/// \note You can override the methods in this class, however, it is not reccomended
	/// to inherit from. As the GameStateStack will not have pointers to your actual game object.
	/// \see StatedGame if you wish to have a custom stated game
	/// \author Miguel Martin
	template <typename TEngineConcept>
	class DefaultStatedGame
		: public StatedGame<TEngineConcept, DefaultStatedGame<TEngineConcept> >
	{
	};
}

#endif // __PINE_GAMELOOP_HPP__
