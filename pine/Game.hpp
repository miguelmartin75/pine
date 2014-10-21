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
        template <class TEngine>
        struct GameWithEngine
        {

        public:

            using Base = GameWithEngine<TEngine>;

            using Game = TGame;
            using Engine = TEngine;

            GameWithEngine() : 
                _engine(nullptr)
            {
            }

            void quit(int errorCode)
            {
                engine().shutdown(errorCode);
            }

            void initialize(int argc, char* argv[])
            {
                engine().initialize(argc, argv);
            }

            void frame_start() 
            { 
                engine().frame_start();
            }

            void update(Seconds deltaTime)
            {
                engine().update(deltaTime);
            }

            void frame_end()
            {
                engine().frame_end();
            }

            void engine(Engine& engine) 
            { 
                assert(!_engine && "engine is already attached to GameWithEngine!");
                _engine = &engine;
            }

            Engine& engine() const { return *_engine; }

            int error_state() const { return engine().error_state(); }
            bool running() const { return !engine().shutdown(); }

        private:

            Engine* _engine;
        };

        struct GameWithoutEngine
        {
        public:

            using Base = GameWithoutEngine;
            using Engine = void; // to signify there is no engine

            GameWithoutEngine() :
                _error_code(0),
                _running(true)
            {
            }

            Game& game() { return *static_cast<Game*>(this); }
            const Game& game() const { return *static_cast<const Game*>(this); }

            int error_state() const { return _error_code; }
            bool running() const { return _running; }

            void quit(int errorCode)
            {
                _error_code = errorCode;
                _running = false;
            }

            void initialize(int argc, char* argv[]) {}
            void frame_start() {}
            void update(Seconds deltaTime) {}
            void frame_end() {}

        private:

            int _error_code;
            bool _running;
        };

        template <class TEngine>
        struct GameTypeDeducer
        {
            using type = GameWithEngine<TEngine>;
        };

        struct GameTypeDeducer<void>
        {
            using type = GameWithoutEngine;
        };
    }

    template <class TGame, class TEngine = void>
    using Game = typename detail::GameTypeDeducer<TGame, TEngine>::type;
}

#endif // PINE_GAME_HPP
