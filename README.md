# pine

Pine is a general, lightweight, header-only C++ library, which is designed to make organisation of a game much simpler.

## Author

Miguel Martin - [miguel.martin.7.5@hotmail.com](mailto:miguel.martin7.5@hotmail.com)

## Tested Compilers

- clang 3.1/Apple Clang Version 4.1 (LLVM 3.1svn)

## Dependencies

- chrono
	- either `boost::chrono` or `std::chrono`, you may change this in configuration* file.

*see below on how to configure the library


## Installation 
To install the library, simply drop and drag the headers into your project file (or reference to them on your local system). Since this library is header-only, there is no need to compile :)

## Configuration
You may configure the library to suit your needs. For example, this library depends on `chrono`, however you may use either boost or the C++11 standard's implementation of the library, it is your choice.

To configure the library, look in the `config.hpp` file in the `pine` directory. Alter the macros present in there to change the library's settings, a comment above each macro should describe what each one does.

## Basic Usage

The Pine Game Framework is simply a framework to help orgnisation of your games. In order to do that, it requires heavy use of C++ templates throughout the code; but do not worry, as typedef's are used frequently through the source code. All code is defined in the `pine` namespace, and it is assumed in this basic tutorial that we are using the `pine` namespace.

### Contents

1. Template Concepts
2. Library Basics
3. The Game Loop
4. The Game
5. The Engine
6. Putting Everything Together

### Concepts

Since C++ templates are used, concepts are used frequently in the library. A concept is basically a set of rules that a class must comply to, i.e. it must define a specific list of methods.

### Library Basics

The library is split up into 3 major basic classes/principles:

- The Game Loop
	- This class describes the "heart-beat" of your game. As imlpied, it controls when the game is updated, drawn, etc.
- The Game
	- This class describes your game and what it contains.
- The Engine
	- The class describes the engine used by your game. By "Engine" it is simply means the lower-level details of the game, e.g. rendering, sound output, etc.

The game loop and game classes are provided for you, however the engine is not entirely provided for you. It is your job to create your own engine for your game, as this provides more flexability.

### The Game Loop

The game loop defines the heart-beat of your game. It takes total control of everything that occurs within your game. A game loop is defined by the `GameLoop<TGameConcept>` class, where `TGameConcept` is a game concept. Please see below for details.

In order to create a game loop, simply supply the game that is connected to the game loop. Then call `run()` to run the loop, or `update()` if you must do other things or do not have control total control of the game loop (e.g. call-backs to update/draw). 

> #### NOTES:
> 1. If you need to use the `update()` method, before calling `update()`, please make sure you have initialized the game loop, via the `initialize()` method.
> 2. I may make game loops more customizeable in the future (i.e. you can implement them easier, rather than changing my code)

Please see the documentation for further details about the methods, such as setting the simulation frame rate and calculating the frame rate of the game.

e.g.

#### Using `run()`

```c++
int main(int argc, char* argv[])
{
   	MyGame game;
   	GameLoop<MyGame> loop(game);
   	
   	return loop.run();
}
```
	
#### Using `update()`

```c++
int main(int argc, char* argv[])
{
	MyGame game;
	GameLoop<MyGame> loop(game);
		
	loop.initialize(); // required to call before calling update()
		
	while(loop.isRunning())
	{
		loop.update(); // this will handle all the nasty details
	}
	
	return loop.getErrorCodeState();
}
```
	
### The Game

A game is what represents your game. There is a game class already defined in the library, that does not require to be inherited from. However, you may define your own Game concept, by implementing the methods below in the Concepts section.

#### Concept

- `begin()`
	- Occurs at the beginning of a frame
- `update(Seconds deltaTime)`
	- Updates the game
- `end()`
	- Occurs at the end of a frame
		
This concept you should not be totally concerned about, as the Game is already defined for you. However, please do note, you may define your own Game concept, but I reccomend not to, as it is much easier to use the pre-built class. The only reason you should define your own game concept is to change the implementation of the game class.

#### Using the Game class

There is a class pre-defined that you may used, called `Game<TEngineConcept>`, where `TEngineConcept` is an engine concept (see "The Engine" for details). The Game class contains: an engine (your engine concept), a game handler, and a game loop. The game loop and engine of the game may be modified through the getters.

A game handler is a simple class that is used for pre and post initialization of your game, it is defined so that you do not have to derive from the game class. Pre initialization is set up, e.g. setting window settings, and post initializatio is for any initilization that must occur after the game has been initialized. See the `GameHandler<TGame>` class for details. The Game class defines a typedef for a GameHandler (referred to as `Handler`) for an easier method to make your own handler classes.

e.g.

```c++
#include "MyEngine.h"
	
class PongGameHandler
	: public MyEngine::Game::Handler
{
 	// ...
};
```

### The Engine

The engine part of the library is mainly defined from you. However, you still require to use the classes defined by the library in order for your engine to work with the pre-built Game class.

#### Concept
This is the main concept you should be concerned about, which is the engine concept. As the Game concept is already defined for you.

- `initialize(int argc, char* argv[])`
	- Initializes the engine, with command line arguments
- `begin()`
   	- Occurs at the beginning of a frame, e.g. clear the screen
- `update(Seconds deltaTime)`
   	- Updates the game, note that this may be called multiple times per frame. i.e. Don't draw here
- `end()`
    - Occurs at the end of a frame, i.e. draw and swap buffers
- `shutDown(int errorCode)`
    - Occurs on shut down of the engine and game, i.e. free textures

#### Defining your own Engine

To define your own engine, you must inherit from the `GameEngine<TEngineConcept>` class (if you using the built in `Game` class) Where `TEngineConcept` is actually the class you are defining at the moment, e.g.

```c++
#include "pine/GameEngine.h"

class MyEngine 
	: public pine::GameEngine<MyEngine>
{
	// ...
};
```

To add custom functionality, you must define the concept methods, but it is reccomended call the base concept methods if you do so. Use the `Base` typedef to refer to the base class.

### Game States

The library also defines a little bit of extra game management. This extra feature is called: game states (defined by the `GameState` class). A game state is simply a "state" in your game, this could be a main menu, the game itself, the pause menu, or anything you want. All GameStates have a reference to your engine and game.

There is a specific engine that helps you integrate game states in your game, the `GameStateEngine<TEngineConcept>`. This engine simply stores a GameStateStack that you can be accessed by your game. Use this class if you wish to define your game that interacts with game states natively.

All GameStates are usually stored as a stack. The last one that is pushed onto the stack will be updated. You may use a `std::stack` to contain these game states, however there is a `GameStateStack` class to help you quite a lot. The `GameStateStack` has some extra pushing options that you may require for a game.

#### Defining your own GameStates

To define your own GameState, you must derive from the abstract class `GameState<TEngineConcept>` and override the virtual methods:

- `initialize()`
    - Used to initalize the GameState
- `update(Seconds deltaTime)`
	- Used to update the GameState
- `draw()`
	- Used for drawing things on screen
- `loadResources()`
 	- Used for loading resources for a GameState
- `unloadResources()`
	- Used for unloading resources for a GameState
	
Luckily, there are typedef's within the `GameStateEngine` for an appropriate `GameState`. This typedef is the `GameStateEngine<T>::GameState` typedef, you may refer to it directly from your own engine class.

e.g.

```c++
#include "MyEngine.h"
    
class MainMenu 
	: public MyEngine::GameState
{
   	// ...
};
```
    
### Putting Everything Together

To put everything together is quite simple. Typically all it takes is 5 lines, or less, in the main function, under most circumstances.

First you must define your engine, using either a `GameStateEngine` or a plain `GameEngine`, and then decide whether you're using the pre-built game class (which I reccomend doing). If you are using the pre-built game class, you must also define a GameHandler, which is simple enough (see above for details).

Once you're done with that, you put everything together quite simply.

e.g.

```c++
int main(int argc, char* argv[])
{
	PongGameHandler handler;
	MyEngine engine;
	MyEngine::Game game(argc, argv, handler, engine);

	return game.run();
}
```

Typically the only line that will change from game-to-game is the type of Game Handler you are using. It may also be the type of engine, if you think you may require a different engine for a different type of game.

That's it. Go code!