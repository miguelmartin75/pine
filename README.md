# pine

Pine is a general, lightweight, header-only C++ library, which is designed to make organisation of a game much simpler.

# Tested Compilers

- clang 3.1/Apple Clang Version 4.1 (LLVM 3.1svn)

# Getting Started

## Dependencies
- chrono (optional)
	- either `boost::chrono` or `std::chrono`, you may change this in the configuration* file.

*see below on how to configure the library


## Installation 
To install the library, simply drop and drag the headers into your project file (or reference to them on your local system). Since this library is header-only, there is no need to compile :)

## Configuration
You may configure the library to suit your needs. For example, this library depends on `chrono`, however you may use either boost or the C++11 standard's implementation of the library, it is your choice.

To configure the library, look in the `config.hpp` file in the `pine` directory. Alter the macros present in there to change the library's settings, a comment above each macro should describe what each one does.

# Basic Usage

The Pine Game Framework is simply a framework to help orgnisation of your games. In order to do that, it requires heavy use of C++ templates throughout the code; but do not worry, as typedef's are used frequently through the source code. All code is defined in the `pine` namespace, and it is assumed in this basic tutorial that we are using the `pine` namespace.

## Concepts
Since this library is quite heavily based off of templates, it uses something called "concepts". A concept is basically an interface, but it instead of it being able to be changed during run-time, it can only be changed during compile time. Here is an overview of all the concepts present within the library:

- Game
	- `initialize(Engine&, int argc, char* argv[])`
		- Initializes the game
	- `begin()`
		- Called at the begining of a frame
	- `end()`
		- Called at the end of a frame
	- `update(Seconds deltaTime)`
		- Used for updating the game
- Engine
	- `initialize(int argc, char* argv[])`
		- Initializes the engine
	- `begin()`
		- Called at the begining of a frame
	- `end()`
		- Called at the end of a frme
	- `update(Seconds deltaTime)`
		- Updates the engine
	- `shutdown(int errorCode)`
		- Shutdowns the engine
		
>### **NOTE**
>Typcically you should called the base's original method if you override a function, unless the documentation does not say to do so.

## Creating an Engine

To create an engine, you must first derive from the base `Engine<TEngineConcept>` class, which defines the Engine concept described above. It has one template parameter (`TEngineConcept`), and it is just the name of your actual engine. It follows the curiously recurring template pattern ([CRTP]) pattern.

#### EXAMPLE

```c++
class MyEngine
	: public pine::Engine<MyEngine>
{
	/* ... */ 
};

```

Typically you only need to create your engine once, and it can be used for multiple types of games. Thus it is reccomended to put re-useable code within an engine, so that it does not need to be altered for another game. However, there is still a possibility that you may need to create another engine or modify an exsisting one for your game.

## Creating a Game

To create a game, you must inherit from `Game<TEngineConcept>`, where `TEngineConcept` is the engine you wish to use with your game. Thus you must have an engine ready to use.

#### EXAMPLE

```c++
#include "MyEngine.hpp"

class MyGame
	: public pine::Game<MyEngine>
{
	/* ... */ 
};
```

Your game class is bound to your engine, that is, it is dependent on the engine you are using. Also, you typically change your game class depending on the game you are making. However, it is possible to have the same game class for all your games.

## Game States

A game state, is a state that is within your game. This could be a pause menu, the "play" screen, the main menu, or anything else that has it's own independent state within your game. 

### The `GameState` class

Game states are implemented as a virtual interface, as they may be changed during run-time. The overrideable methods of a game state are as follows:

- `initialize()`
	- Used to initialize the game state; is the first method to be called
- `update(Seconds deltaTime)`
	- Used for updating the game state
- `draw()`
	- Used for drawing
- `loadResources()`
	- Used for loading resources
- `unloadResources()`
	- Used for unloading (destroying) resources

If you wish to create a game state, then you can use the `GameState` class. However, this is a template class, which requires a game and an engine, so it can be made. Since it requires a game and an engine (`pine::GameState<TGameConcept, TEngineConcept>`), it is typically easier to use the typedef's that are located within the library. Please see "Integrating Game States with your Game class". Here is an example of not using any typedefs to create your own game state (**note:** this is not reccomended to do):

#### EXAMPLE

```c++
#include <pine/GameState.hpp>

#include "MyGame.hpp"
#include "MyEngine.hpp"

class PlayGameState
	: public pine::GameState<MyGame, MyEngine>
{
	/* ... */
};
```

### The `GameStateStack` class

The `GameStateStack` class, is a class that manages game states for you. It handles them within a stack, as this makes stacks easier to use and integrate with each other. The most common methods that you will use are:

- `push(GameState*, PushType)`
	- pushes on a new game state on the stack
- `pop()`
	- pops the statck
- `clear()`
	- clears the stack
- `update(Seconds deltaTime)`
	- calls update to the necessary game states in the stack
- `draw()`
	- calls draw to the necessary game states in the stack

#### PushType enum

The `PushType` enum is used to determine how you want to push your states on the stack. You may provide this with the `push` method for a `GameStateStack`.

- `PushAndPop` (`Default`)
	- Pushes the stack and pops off the last stack
- `PushAndPopAllPreviousStates`
	- Pushes the stack and pops off all previous game states
- `PushWithoutPopping`
	- Pushes the stack without popping any previous game states
- `PushWithoutPoppingSilenty`
	- "Silently" pushes the stack without popping any previous game states. Silently implies that the previous state will be updated.
	
	
Typically you do not create your own GameStateStack object, but in case you wish to do so, it requires a game and an engine, much like a GameState. Like so:

#### EXAMPLE

```c++
GameStateStack<MyGame, MyEngine> gameStateStack;
```

### Integrating Game States with your Game class

To integrate a game state with your game class, you have three options:

1. Manually handle the states with your Game class
2. Use a `GameStateStack` object in your Game class
3. Inherit from `StatedGame` within the `StatedGame.hpp` header file.

The first option is only reccomended if the code provided within the library does not offer what you wish to achieve.

The second option is not reccomended at all, as the third option does this, but allows you to type only four characters more. Thus it is reccomended that you use the StateGame class instead, it works just like `pine::Game`; infact it inherits from `pine::Game`. The only difference is, you must supply the name of your game class along with the engine you are using in the template paramters.

#### EXAMPLE

```c++
class MyGameThatUsesStates
	: public StatedGame<MyGameThatUsesStates, MyEngine>
{
	/* ... */
};

// ... using the game

getGame().getGameStateStack().push<PlayGameState>(); 
```

>#### NOTE
>The StatedGame class has a typedef for game states, called `State`. Thus
>if you wish to create your own states, it is reccomended you do so by inheriting
>from `MyGame::State`, where `MyGame` is your game class.

## Running your Game

In order to your game, you have two options:

1. Create your own game loop for your game, manually.
2. Use the RunGame templated function in the `pine/RunGame.hpp` header file.

If you chose the latter, then it's quite simple to start your game up. Simply use of the following overloads:

### `RunGame<TGameConcept>(<optional-command-args>)`

- Allocates an engine on the stack
- Allocates your game class on the stack
- Initializes your game class object with the (optional) command line arguments
- Runs your game
	
#### EXAMPLE
```c++

#include <pine/RunGame.hpp>

#include "MyGame.hpp"

int main(int argc, char* argv[])
{
	return RunGame<MyGame>(argc, argv);
}
```

### `RunGame<TGameConcept>(TEngineConcept& engine, <optional-command-args>)`
- Allocates your game class on the stack
- Initializes your game class object with the (optional) command line arguments
- Runs your game
	
#### EXAMPLE
```c++

#include <pine/RunGame.hpp>

#include "MyGame.hpp"
#include "MyEngine.hpp"

int main(int argc, char* argv[])
{
	MyEngine engine;
	return RunGame<MyGame>(engine, argc, argv);
}
```

### `RunGame(TGameConcept& game)`
- Runs your game, without initializing or allocating an engine or game
	
#### EXAMPLE
```c++

#include <pine/RunGame.hpp>

#include "MyGame.hpp"
#include "MyEngine.hpp"

int main(int argc, char* argv[])
{
	MyEngine engine;
	MyGame game;
	game.initialize(engine, argc, argv);
	
	return RunGame(game);
}
```

# License
Copyright (C) 2013 Miguel Martin (miguel.martin7.5@hotmail.com)

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in
all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
THE SOFTWARE.

[CRTP]: http://en.wikipedia.org/wiki/Curiously_recurring_template_pattern
