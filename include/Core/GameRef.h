#ifndef GAMEREF_H
#define GAMEREF_H

#include "Game.h"

class GameRef
{
    public:
        GameRef(Game *game);
        virtual ~GameRef();

        Game *getGame(){return mGame;}

    protected:
    private:
        Game *mGame;
};

#endif // GAMEREF_H
