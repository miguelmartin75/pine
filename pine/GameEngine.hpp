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

#ifndef __PINE_GAMEENGINE_HPP__
#define __PINE_GAMEENGINE_HPP__

#include "Game.hpp"

namespace pine
{
	/// \brief A base class for a plain Engine concept
	/// \tparam TEngineConcept The Engine you are defining,
	///							i.e. the class that inherits from this class
	///
	/// The GameEngine class is a base class for all Engine
	/// classes. An Engine is a concept, that is described by these
	/// methods:
	///
	/// - initialize(int argc, char* argv[])
	/// - begin()
	/// - update(Seconds deltaTime)
	/// - end()
	/// - shutDown(int errorCode)
	///
	/// You are required to inherit from this class, as it has a reference
	/// to a game object that the Engine is connected to. You are not
	/// required to provide every method for the Engine concept, as
	/// it is already defined in this class. Please take note that
	/// they do not do anything in the base class for a game engine.
	///
	/// Please look further down to see the documentation of specific methods.
	///
	///
	/// \author Miguel Martin
	template <class TEngineConcept>
	class GameEngine
	{
	public:
		
		friend Game<TEngineConcept>;
		typedef Game<TEngineConcept> Game;
		typedef GameEngine<TEngineConcept> Base;
		
		/// Default Constructor
		GameEngine()
			: _game(0 /* NULL */),
			  _isInitialized(false)
		{
		}
		
		/// Destructor
		~GameEngine()
        {
        }
		
		
		
        /// \return The Game that the Engine is connected to
		Game& getGame()
		{ return *_game; }
		
        /// \return The Game that the Engine is connected to
		const Game& getGame() const
		{ return _game; }
		
        /// \return true if the Engine is initalized, false otherwise
		bool isInitialized() const
		{ return _isInitialized; }
		
		
		/********************************************************
		 * In order to add custom functionality, you must
		 * override these methods in a derived class.
		 *******************************************************/
		
        /// Initializes the Engine
        /// \param argc The amount of command line arguments
        /// \param argc Command line arguments
		void initialize(int argc, char* argv[]) {}
		
        /// Used for the beginning of a frame
		void begin() {}
		
        /// Updates the Engine
        /// \param deltaTime The change in time
        /// \note
        /// Do not use this for drawing, as the default game loop may
        /// call this method multiple times per frame.
		void update(Seconds deltaTime) {}
		
        /// Ends a frame
        /// \note
        /// Use this for drawing
		void end() {}
		
        /// Shut downs the engine
        /// \param errorCode The error code that the
        ///                  engine will shutdown with
        void shutDown(int errorCode) { }
        
	protected:
		
		/// Call this when you're done initializing
        /// \note
        /// You are required to call this method
		void finalizeInitialization()
		{
			_isInitialized = true;
		}
		
	private:
		
		// called by the Game class
		void setGame(Game* game)
		{ _game = game; }
		
        /// A reference the Game the Engine is attached to
		Game* _game;

	    /// Determines if the engine has been initialized	
		bool _isInitialized;
	};
}

#endif // __PINE_GAMEENGINE_HPP__
