#pragma once

#include <stdio.h>
#include <stdlib.h>
#include "GameEngine.hpp"


//------------------------------------------------------------------------------------
// Program main entry point
//------------------------------------------------------------------------------------
int main(void) 
{
    GameEngine::start("./config.txt");
    GameEngine::run();

    return 0;
}

