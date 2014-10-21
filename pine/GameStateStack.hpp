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

#ifndef PINE_GAMESTATESTACK_HPP
#define PINE_GAMESTATESTACK_HPP

#include <vector>
#include <memory>
#include <utility>
#include <algorithm>

#include <cassert>

#include <pine/GameState.hpp>

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
        
		virtual void onGameStateWillBePushed(TGameStateStack& sender, typename TGameStateStack::GameState& gameState) {}
		virtual void onGameStateWasPushed(TGameStateStack& sender, typename TGameStateStack::GameState& gameState) {}
		virtual void onGameStateWillBeRemoved(TGameStateStack& sender, typename TGameStateStack::GameState& gameState) {}
		virtual void onStackWillBePopped(TGameStateStack& sender) {}
		virtual void onStackWillBeCleared(TGameStateStack& sender) {}
	};
	
	/// \brief Resembles a stack of game states
	/// \tparam TEngineConcept An Engine concept, which derives from GameEngine
	/// \author Miguel Martin
	template <class TGameConcept>
	class GameStateStack
	{
	public:
		
		typedef TGameConcept Game;
		typedef GameStateStack<Game> ThisType;
		typedef GameState<Game> GameState;
		typedef GameStateStackListener<ThisType> Listener;
		
		explicit GameStateStack(GameState* gameState = nullptr)
		{
			if(gameState) push(gameState);
		}
		
		GameStateStack(const GameStateStack&) = default;
		GameStateStack(GameStateStack&&) = default;
        GameStateStack& operator=(const GameStateStack&) = default;
        GameStateStack& operator=(GameStateStack&&) = default;
		
		~GameStateStack() { clear(); }
        
		template <typename TGameState, class... Args>
		void push(Args&&... args)
        {
            push(new TGameState{std::forward<Args>(args)...});
		}
		
		/// Pushes a GameState on the stack
		/// \param gameState The GameState you wish to add on the stack (should be allocated on the free-store [heap])
		/// \param pushType The PushType that you wish to push the GameState with
		/// \see PushType for details
		void push(GameState* gameState, PushType pushType = PushType::Default)
		{
			assert(gameState && "GameState is null, please offer a non-null GameState");
			
            for(auto& listener : _listeners)
			{
				listener->onGameStateWillBePushed(*this, gameState);
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
			
			// load resources
			gameState->loadResources();
            
			// initialize the state
			gameState->initialize();
		}
		
		/// Pops the GameState stack
		void pop()
		{
			if(_stack.empty()) return; 
			
			for(auto& listener : _listeners)
			{
				listener->onStackWillBePopped(*this);
			}
			
			_stack.pop_back();
		}

        void frame_start() 
        { 
            perform_f_on_stack([](GameState* state) { state->frame_start(); });
        }
		
		void update(Seconds deltaTime)
		{
            perform_f_on_stack([](GameState* state) { state->update(); });
		}
		
		void frame_end()
		{
            perform_f_on_stack([](GameState* state) { state->frame_end(); });
		}
		
		/// Clears the GameStateStack
		void clear()
		{
            for(auto& listener : _listeners)
			{
				listener->onStackWillBeCleared(*this);
			}
			
			_stack.clear();
		}
		
		/// Removes a GameState from the stack
		/// \param gameState The GameState you wish to remove
		void remove(GameState* gameState)
		{
			assert(gameState);

            auto elementToRemove = std::find_if(_stack.begin(), _stack.end(), [](GameStatePair& p) -> bool
                    {
                        return p.first.get() == gameState;
                    });

            if(elementToRemove == _stack.end())
                return;

            for(auto& listener : _listeners)
            {
                listener->onGameStateWillBeRemoved(*this, gameState);
            }

            _stack.erase(elementToRemove);
		}
		
		
		/// \return The Game that the GameStack is connected to
		Game& getGame() const { return *_game; }
		
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


        template <typename F>
        void perform_f_on_stack(F f)
        {
			// we're going to loop through the stack backwards
			// if the top is silently pushed on, we will iterate again
			for(size_t i = _stack.size(); i-- > 0;)
			{
				f(_stack[i].first);
				
				// if we no longer need to continue to iterate
				if(_stack[i].second != PushType::PushWithoutPoppingSilenty)
				{
					break; // break out of the loop
				}
			}
        }
		
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

		void setGame(Game& game) { _game = &game; }
	};
}

#endif // PINE_GAMESTATESTACK_HPP
