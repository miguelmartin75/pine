#include <iostream>

#include <pine/RunGame.hpp>
#include <pine/Game.hpp>
#include <pine/Engine.hpp>
#include <pine/StatedGame.hpp>

struct MyEngine : pine::Engine<MyEngine>
{
public:

    void onInit(int argc, char* argv[]) { std::cout << "Engine init\n"; shutdown(33); }
    void onFrameStart() { std::cout << "Engine frame start\n"; }
    void onUpdate(pine::Seconds deltaTime) { std::cout << "Engine update\n"; }
    void onFrameEnd() { std::cout << "Engine frameEnd\n"; }
    void onShutdown()
    {
        std::cout << "shutdown nigga: " << getErrorState() << '\n';
    }
};

struct MyGame : pine::StatedGame<MyGame, MyEngine>
{
    void onInit(int argc, char* argv[]);
    void onFrameStart() 
    {
        std::cout << "--Frame Start\n";
    }
    void onUpdate(pine::Seconds deltaTime)
    {
        std::cout << "--Game Update\n";
    }
    void onFrameEnd() 
    { 
        std::cout << "--Game Frame end\n";
        quit(1);
    }
    void onWillQuit(int errorCode)
    {
        std::cout << "--will quit with error code: " << errorCode << '\n';
    }
};


struct MyState : public MyGame::State
{
private:

    virtual void init() override
    {
        std::cout << "---initing state\n";
    }
    virtual void loadResources() override
    {
        std::cout << "---loading resources in state\n";
    }
    virtual void unloadResources() override
    {
        std::cout << "---unloading resources in state\n";
    }
    virtual void frameStart() override
    { 
        std::cout << "---starting frame in state\n";
    }

    virtual void update(pine::Seconds deltaTime) override
    {
        std::cout << "---updating state in state\n";
    }

    virtual void frameEnd() override 
    {
        std::cout << "---ending frame in state\n";
        getGame().quit(2);
    }

};

void MyGame::onInit(int argc, char* argv[])
{
    std::cout << "--Game init\n";
    getStateStack().push<MyState>();
}

int main(int argc, char *argv[])
{
    return pine::RunGame<MyGame>(argc, argv);
}
