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

#ifndef PINE_ENGINE_HPP
#define PINE_ENGINE_HPP

#include <pine/time.hpp>

namespace pine
{
    template <class TEngine>
    class Engine
    {
    public:

        typedef Engine<TEngine> Base;

        Engine() :
            _errorState(0),
            _hasShutdown(false)
        {
        }

        // fake "pure virtual functions"
        void initialize(int argc, char* argv[]);
        void frameStart();
        void update(Seconds deltaTime);
        void frameEnd();
        void onShutdown();

        void shutdown(int errorCode) 
        {
            _errorState = errorCode;
            _hasShutdown = true;

            static_cast<TEngine*>(this)->onShutdown();
        }

        int getErrorState() const { return _errorState; }
        bool hasShutdown() const { return _hasShutdown; }

    private:

        int _errorState;

        bool _hasShutdown;
    };
}

#endif // PINE_ENGINE_HPP
