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

#ifndef __PINE_ENGINE_HPP__
#define __PINE_ENGINE_HPP__

#include <pine/WithEngine.hpp>

namespace pine
{
	/// \brief A base class for a plain Engine concept
	/// \tparam TEngineConcept The Engine you are defining,
	///							i.e. the class that inherits from this class
	///
	/// The Engine class is a base class for an Engine class you may define.
    /// The main reason for this class is to provide a basis for which you can extend,
    /// that is so you don't have to rewrite code. 
	/// An Engine is a concept, that is described by these methods:
	///
	/// - initialize(int argc, char* argv[])
	/// - frame_begin()
	/// - update(Seconds deltaTime)
	/// - frame_end()
	/// - shutdown(int errorCode)
	///
	/// \author Miguel Martin
	template <class TEngineConcept>
	class Engine
	{
    public:

        typedef TEngineConcept Engine;
    private:

        Engine& actual_engine() { return *static_cast<Engine*>(this); }
        const Engine& actual_engine() const { return *static_cast<const Engine*>(this); }

	public:
		
		friend GameWithEngine<TEngineConcept>;
        
		typedef Game<TEngineConcept> BaseGame;
		typedef Engine<TEngineConcept> Base;
		
		/// Default Constructor
		Engine() : 
            _game(nullptr)
		{
		}
		
		/// \return The Game that the Engine is connected to
        /// \note You will have to cast the return value
        ///       if you want to do something special with it
		Game& game()
		{ return *_game; }
		
		/// \return The Game that the Engine is connected to
		const Game& getGame() const
		{ return *_game; }
		
		/// Initializes the Engine
		/// \param argc The amount of command line arguments
		/// \param argv Command line arguments
		void initialize(int argc, char* argv[])
        {
            actual_engine().initialize(argc, argv);
        }
		
		/// frame_begin() is called every frame before anything occurs
		/// \note It is recommended to event handling here.
		void frame_begin()
        {
            actual_engine().frame_begin();
        }
		
		/// Updates the Engine
		/// \param deltaTime The change in time
		/// \note
		/// Do not use this for drawing, as the default game loop may
		/// call this method multiple times per frame.
		void update(pine::Seconds deltaTime)
        {
            actual_engine().update(deltaTime);
        }
		
		/// frame_end() is called at the end of every frame
		/// \note It is recommended to do rendering here.
		void end() 
        {
            actual_engine().frame_end();
        }
		
		/// Shut downs the engine
		/// \param errorCode The error code to shutdown with
		void shutdown(int errorCode) 
        {
            actual_engine().shutdown(errorCode);
        }
		
	private:
		
		// called by the Game class
		void setGame(Game* game) { _game = game; }

        /// \return true if the Engine is attached to a Game already
        bool isAttachedToGame() const { return _game != nullptr; }
		
		/// A reference the Game the Engine is attached to
		Game* _game;
	};
}

#endif // __PINE_GAMEENGINE_HPP__
