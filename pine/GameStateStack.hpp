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

#ifndef __PINE_GAMESTATESTACK_HPP__
#define __PINE_GAMESTATESTACK_HPP__

#include <vector>
#include <memory>
#include <cassert>

namespace pine
{
	/// \brief An enumeration used for a stack
	///
	/// This describes different ways to push
	/// a game state on a stack
	enum class PushType
	{
		/// Pushes on a new GameState, and pops off the
		/// previous GameState that was pushed on the stack
		/// (if there is a previous GmaeState).
		PushAndPop,
		
		/// Pushes on a new GameState,
		/// and pops off all previous states on the stack
		PushAndPopAllPreviousStates,
		
		/// Pushes on a new GameState,
		/// without popping off the previous GameState
		PushWithoutPopping,
		
		/// Pushes on a new GameState, without popping off the last GameState
		/// Unlike the previous enumeration, this enumeration makes it so that
		/// the previous GameState is still updated, whilst the new GameState is updated
		PushWithoutPoppingSilenty,
		
		/// The default push type
		Default = PushAndPop
	};
	
	/// \brief Listens to events that a GameStateStack notifies out
	/// \author Miguel Martin
	template <class TGameStateStack>
	class GameStateStackListener
	{
        friend TGameStateStack;
        
	public:
		
		virtual ~GameStateStackListener() {}
        
	private:
        
		virtual void onGameStateWillBePushed(TGameStateStack& sender, typename TGameStateStack::GameState* gameState) {}
		virtual void onGameStateWasPushed(TGameStateStack& sender, typename TGameStateStack::GameState* gameState) {}
		virtual void onGameStateWillBeRemoved(TGameStateStack& sender, typename TGameStateStack::GameState* gameState) {}
		virtual void onStackWillBePopped(TGameStateStack& sender) {}
		virtual void onStackWillBeCleared(TGameStateStack& sender) {}
	};
	
	template <class TEngine>
	class GameStateEngine; // reuqired for linker errors
	
	/// \brief Resembles a stack of game states
	/// \tparam TEngineConcept An Engine concept, which derives from GameEngine
	/// \author Miguel Martin
	template <class TGameConcept, class TEngineConcept>
	class GameStateStack
	{
	public:
		
		typedef TEngineConcept Engine;
		typedef TGameConcept Game;
		typedef GameStateStack<Game, Engine> ThisType;
		typedef GameState<Game, Engine> GameState;
		typedef GameStateStackListener<ThisType> Listener;
		
		explicit GameStateStack(GameState* gameState = nullptr)
		{
			if(gameState)
			{
				push(gameState);
			}
		}
		
		GameStateStack(const GameStateStack& gameStateStack)
			: _listeners(gameStateStack._listeners),
			  _stack(gameStateStack._stack),
			  _game(gameStateStack._game)
		{
		}
		
		GameStateStack(GameStateStack&& gameStateStack)
			: _listeners(std::move(gameStateStack._listeners)),
			  _stack(std::move(gameStateStack._stack)),
			  _game(std::move(gameStateStack._game))
		{
			gameStateStack._game = nullptr;
		}
		
		~GameStateStack()
		{
			clear(); // clear the stack
		}
        
		template <typename TGameState
#ifdef PINE_USE_VARIADIC_TEMPLATES
		, typename... Args
#endif // PINE_USE_VARIADIC_TEMPLATES
		>
		void push(
#ifdef PINE_USE_VARIADIC_TEMPLATES
				Args&&... args
#endif // PINE_USE_VARIADIC_TEMPLATES
				)
        {
            push(new TGameState
#ifdef PINE_USE_VARIADIC_TEMPLATES
			{args...}
#endif // PINE_USE_VARIADIC_TEMPLATES
        		);
		}
		
		/// Pushes a GameState on the stack
		/// \param gameState The GameState you wish to add on the stack (should be allocated on the free-store [heap])
		/// \param pushType The PushType that you wish to push the GameState with
		/// \see PushType for details
		void push(GameState* gameState, PushType pushType = PushType::Default)
		{
			assert(gameState && "GameState is null, please offer a non-null GameState");
			
			for(auto i = _listeners.begin(); i != _listeners.end(); ++i)
			{
				(*i)->onGameStateWillBePushed(*this, gameState);
			}
			
			switch(pushType)
			{
				case PushType::PushAndPop:
					pop();
					break;
				case PushType::PushAndPopAllPreviousStates:
					clear();
					break;
				default:
					break;
			}
			
			_stack.push_back(GameStatePair(GameStatePtrImpl(gameState), pushType));
			gameState->_game = _game;
			
			// initialize the state
			gameState->initialize();
            
			// load resources
			gameState->loadResources();
		}
		
		/// Pops the GameState stack
		void pop()
		{
			if(_stack.empty())
			{
				return;
			}
			
			for(auto i = _listeners.begin(); i != _listeners.end(); ++i)
			{
				(*i)->onStackWillBePopped(*this);
			}
			
			_stack.pop_back();
		}
		
		/// Calls update for appropriate GameStates on the GameStateStack
		/// \param deltaTime The change in time
		void update(Seconds deltaTime)
		{
			// we're going to loop through the stack backwards
			// if the top is silently pushed on, we will iterate again
			for(size_t i = _stack.size(); i-- > 0;)
			{
				_stack[i].first->update(deltaTime);
				
				// if we no longer need to continue to iterate
				if(_stack[i].second != PushType::PushWithoutPoppingSilenty)
				{
					break; // break out of the loop
				}
			}
		}
		
		/// Calls draw for appropriate GameStates on the GameStateStack
		void draw()
		{
			// we're going to loop through the stack backwards
			// if the top is silently pushed on, we will iterate again
			for(size_t i = _stack.size(); i-- > 0;)
			{
				_stack[i].first->draw();
				
				// if we no longer need to continue to iterate
				if(_stack[i].second != PushType::PushWithoutPoppingSilenty)
				{
					break; // break out of the loop
				}
			}
		}
		
		/// Clears the GameStateStack
		void clear()
		{
			for(auto i = _listeners.begin(); i != _listeners.end(); ++i)
			{
				(*i)->onStackWillBeCleared(*this);
			}
			
			_stack.clear();
		}
		
		/// Removes a GameState from the stack
		/// \param gameState The GameState you wish to remove
		void remove(GameState* gameState)
		{
			assert(gameState);
			
			for(auto i : _stack)
			{
				if(i->first.get() == gameState)
				{
					for(auto i = _listeners.begin(); i != _listeners.end(); ++i)
					{
						(*i)->onGameStateWillBeRemoved(*this, gameState);
					}
					
					_stack.erase(i);
					break;
				}
			}
		}
		
		/// Sets the Game
		/// \param game The Game you wish to have all GameStates to have a reference to
		void setGame(Game& game)
		{ _game = &game; }
		
		/// \return The Game that the GameStack is connected to
		Game& getGame()
		{ return *_game; }
		
		/// Adds a listener to the GameStateStack
		/// \param listener The listener you wish to add to the game state stack
		void addListener(Listener* listener)
		{
			assert(listener);
			_listeners.push_back(listener);
		}
		
		/// Removes a listener to the GameStateStack
		/// \param listener The listener you wish to remove from the game state stack
		void removeListener(Listener* listener)
		{
			assert(listener);
			_listeners.erase(std::remove(_listeners.begin(), _listeners.end(), listener), _listeners.end());
		}
        
	private:
		
		// utility class used to delete game states
		struct GameStateDeleter
		{
			void operator()(GameState* gameState) const
			{
				// unload resources
				gameState->unloadResources();
				
				// delete the game state
				delete gameState;
			}
		};
		
		typedef std::unique_ptr<GameState, GameStateDeleter> GameStatePtrImpl;
		typedef std::pair<GameStatePtrImpl, PushType> GameStatePair;
		typedef std::vector<GameStatePair> StackImpl;
		typedef std::vector<Listener*> ListenerArray;
		
		
		/// Objecst that listen to game state events
		ListenerArray _listeners;
		
		/// The underlying stack implementation
		StackImpl _stack;
		
		/// The game attached to the stack
		Game* _game;
	};
}

#endif // __PINE_GAMESTATESTACK_HPP__