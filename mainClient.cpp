#include <iostream>

#include <assert.h>
#include <cstdlib>
#include <ctime>
#include <fstream>
#include <string>
#include <LTBL/Constructs/Vec2f.h>

#include <Fission.h>
#include "GameState.h"
#include "PlayerControlComponent.h"
#include "EnemyComponent.h"

int main()
{
    Game *game = new Game;
    game->run(new GameState(game, NetworkType::CLIENT));

    return 0;
}
