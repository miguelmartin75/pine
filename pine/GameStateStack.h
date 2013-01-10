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

#ifndef __PINE_GAMESTATESTACK_H__
#define __PINE_GAMESTATESTACK_H__

#include <vector>
#include <memory>
#include <cassert>

namespace pine
{
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
		PushWithoutPoppingSilenty
	};
	
	template <class TGameStateStack>
	class GameStateStackDelegate
	{
	public:
		
		virtual ~GameStateStackDelegate() {}
		
		virtual void onGameStateWillBePushed(TGameStateStack& sender, typename TGameStateStack::GameState* gameState) {}
		virtual void onGameStateWasPushed(TGameStateStack& sender, typename TGameStateStack::GameState* gameState) {}
		virtual void onGameStateWillBeRemoved(TGameStateStack& sender, typename TGameStateStack::GameState* gameState) {}
		virtual void onStackWillBePopped(TGameStateStack& sender) {}
		virtual void onStackWillBeCleared(TGameStateStack& sender) {}
	};
	
	template <class TEngine>
	class GameStateEngine;
	
	template <class TEngine>
	class GameStateStack
	{
	public:
		
		typedef TEngine Engine;
		typedef Game<TEngine> Game;
		typedef GameStateStack<TEngine> ThisType;
		typedef GameState<TEngine> GameState;
		typedef GameStateStackDelegate<ThisType> Delegate;
		
		explicit GameStateStack(GameState* gameState = NULL, Delegate* delegate = NULL)
			: _delegate(delegate)
		{
			if(gameState)
				push(gameState);
		}
		
		GameStateStack(const GameStateStack& gameStateStack)
			: _delegate(gameStateStack._delegate),
			  _stack(gameStateStack._stack),
			  _game(gameStateStack._game)
		{
		}
		
		GameStateStack(GameStateStack&& gameStateStack)
			: _delegate(gameStateStack._delegate),
			  _stack(gameStateStack._stack),
			  _game(gameStateStack._game)
		{
			gameStateStack._delegate = nullptr;
		}
		
		
		/// Pushes a GameState on the stack
		/// \param gameState The GameState you wish to add on the stack
		/// \param pushType The PushType that you wish to push the GameState with
		/// \see PushType for details
		void push(GameState* gameState, PushType pushType = PushType::PushAndPop)
		{
			assert(gameState != nullptr);
			
			if(getDelegate())
			{
				getDelegate()->onGameStateWillBePushed(*this, gameState);
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
			
			_stack.push_back(GameStatePair(gameState, pushType));
			gameState->_game = _game;
			
			// load resources
			gameState->loadResources();
			
			// initialize the state
			gameState->initialize();
		}
		
		/// Pops the GameState stack 
		void pop()
		{
			if(getDelegate())
			{
				getDelegate()->onStackWillBePopped(*this);
			}
			
			_stack.front().first->unloadResources();
			
			_stack.pop_back();
		}
		
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
		
		void clear()
		{
			if(getDelegate())
			{
				getDelegate()->onStackWillBeCleared(*this);
			}
			
			_stack.clear();
		}
		
		/// Removes a GameState from the stack
		/// \param gameState The GameState you wish to remove
		void remove(GameState* gameState)
		{
			assert(gameState != nullptr);
			
			for(auto i : _stack)
			{
				if(i->first.get() == gameState)
				{
					if(getDelegate())
					{
						getDelegate()->onGameStateWillBeRemoved(*this, gameState);
					}
					
					_stack.erase(i);
					break;
				}
			}
		}
		
		void setDelegate(Delegate* delegate)
		{ _delegate = delegate; }
		
		Delegate* getDelegate()
		{ return _delegate; }
		
		void setGame(Game& game)
		{ _game = &game; }
		
		Game& getGame()
		{ return *_game; }
		
	private:
		
		typedef std::unique_ptr<GameState> GameStatePtrImpl;
		typedef std::pair<GameStatePtrImpl, PushType> GameStatePair;
		typedef std::vector<GameStatePair> StackImpl;
		
		
		
		/// The delegate class of the GameStateStack
		Delegate* _delegate;
		
		/// The underlying stack implementation
		StackImpl _stack;
		
		/// The game attached to the stack
		Game* _game;
	};
}

#endif // __PINE_GAMESTATESTACK_H__