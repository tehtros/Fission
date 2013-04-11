#include <iostream>

#include <assert.h>
#include <cstdlib>
#include <ctime>
#include <fstream>
#include <string>
#include <LTBL/Constructs/Vec2f.h>

#include <Fission.h>
#include "GameState.h"

int main()
{
    Game *game = new Game(800, 600);
    game->run(new GameState(game));
    delete game;

    return 0;
}
