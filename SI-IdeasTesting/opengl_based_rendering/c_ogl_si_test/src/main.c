#include <stdio.h>
#include "engine.h"

#define WIDTH 1024
#define HEIGHT 768

int main(int argc, char** argv)
{
    Engine engine;

    engine_construct(&engine, WIDTH, HEIGHT);
    engine_run(&engine);

    return 0;
}