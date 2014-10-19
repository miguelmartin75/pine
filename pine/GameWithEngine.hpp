#ifndef GAMEWITHENGINE_HPP
#define GAMEWITHENGINE_HPP

#include "Game.hpp"

namespace pine
{
    template <class TGame, class TEngineConcept>
    class GameWithEngine : public Game<TGame>
    {
    protected:

        typedef GameWithEngine<TGame> Base;

    public:

        typedef TEngineConcept Engine;

        GameWithEngine() : 
            _engine(nullptr)
        {
        }

        /// Initializes the game
        /// \param engine The engine you wish to initialize the game with
        /// \param argc The number of arguments
        /// \param argv The arguments themself
        /// \note Do not override/hide this function
        /// \see onWillInitialize
        /// \see onInitialized
        void initialize(Engine& engine, int argc, char* argv[])
        {
            assert(engine.isAttachedToGame() != false && "Engine already has a game attached to it!");
            
            _engine = &engine;
            engine().game(this);
            
            engine().initialize(argc, argv);

            initialize(argc, argv);        
        }

        void initialize(int argc, char* argv[])
        {
            assert(_engine && "no engine attached to GameWithEngine");

            actual_game().initialize(argc, argv);
        }

        void frame_start()
        {
            getEngine().frame_start();
            actual_game().frame_start();
        }

        void update(Seconds seconds)
        {
            engine().update(seconds);
            actual_game().update(seconds);
        }

        void frame_end()
        {
            engine().frame_end();
            actual_game().frame_end();
        }

        /// \return The Engine of the game
        Engine& engine() { return *_engine; }
        const Engine& engine() const { return *_engine; }

    private:

        /// The Engine of the game
        Engine* _engine;
    };

}

#endif // GAMEWITHENGINE_HPP
