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

#ifndef PINE_GAME_HPP
#define PINE_GAME_HPP

#include <cassert>

#include <pine/time.hpp>

namespace pine
{
    namespace detail
    {        
        struct GameType { };
        template <class TGame, class TEngine>
        struct GameWithEngine : GameType
        {
        public:

            using Base = GameWithEngine<TGame, TEngine>;
            using Game = TGame; 

            using Engine = TEngine;

            GameWithEngine() : _engine(nullptr) { }

            void configureEngine()
            {
                thisType()->onConfigureEngine();
            }

            void quit(int errorCode)
            {
                if(!isRunning()) return;
                thisType()->onWillQuit(errorCode);
                getEngine().shutdown(errorCode);
            }

            void init(int argc, char* argv[])
            {
                getEngine().init(argc, argv);
                if(!isRunning()) return;
                thisType()->onInit(argc, argv);
            }

            void frameStart()
            { 
                getEngine().frameStart();
                thisType()->onFrameStart();
            }

            void update(Seconds deltaTime)
            {
                getEngine().update(deltaTime);
                thisType()->onUpdate(deltaTime);
            }

            void frameEnd()
            {
                getEngine().frameEnd();
                thisType()->onFrameEnd();
            }

            void setEngine(Engine& engine)
            {
                assert(!_engine && "Engine is already assigned");
                _engine = &engine;
                configureEngine();
            }

            Engine& getEngine() const { return *_engine; }

            int getErrorState() const { return getEngine().getErrorState(); }
            bool isRunning() const { return !getEngine().hasShutdown(); }

        private:

            Game* thisType() { return static_cast<Game*>(this); }
            const Game* thisType() const { return static_cast<const Game*>(this); }

            Engine* _engine;
        };

        template <class TGame>
        struct GameWithoutEngine
        {
        public:

            using Base = GameWithoutEngine;
            using Game = TGame;
            using Engine = void; // to signify there is no engine

            GameWithoutEngine() :
                _errorState(0),
                _isRunning(true)
            {
            }

            int getErrorState() const { return _errorState; }
            bool isRunning() const { return _isRunning; }

            void quit(int errorCode)
            {
                if(!isRunning()) return;

                _errorState = errorCode;
                _isRunning = false;
                thisType()->onWillQuit(errorCode);
            }

            void init(int argc, char* argv[])
            {
                thisType()->onInit(argc, argv);
            }

            void frameStart()
            { 
                thisType()->onFrameStart();
            }

            void update(Seconds deltaTime)
            {
                thisType()->onUpdate(deltaTime);
            }

            void frameEnd()
            {
                thisType()->onFrameEnd();
            }

        private:

            Game* thisType() { return static_cast<Game*>(this); }
            const Game* thisType() const { return static_cast<const Game*>(this); }

            int _errorState;
            bool _isRunning;
        };

        template <class TGame, class TEngine>
        struct GameTypeDeducer
        {
            using type = GameWithEngine<TGame, TEngine>;
        };

        template <class TGame>
        struct GameTypeDeducer<TGame, void>
        {
            using type = GameWithoutEngine<TGame>;
        };
    }

    template <class TGame, class TEngine = void>
    using Game = typename detail::GameTypeDeducer<TGame, TEngine>::type;
}

#endif // PINE_GAME_HPP
