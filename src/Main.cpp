#include "Core/Engine.h"

int main(int argc, char* argv[])
{
    Engine engine;
    engine.Init();
    engine.Run();
    engine.Shutdown();

    return 0;
}