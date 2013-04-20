/*
RefCounted.h
Theodore DeRego
Copyright 2012

The abstract base class for all managers
*/

#ifndef MANAGER_H
#define MANAGER_H

#include "Core/GameRef.h"

class Manager : public GameRef
{
    public:
        Manager(Game *game) : GameRef(game)
        {
            mPaused=false;
        }

        virtual ~Manager(){}

        virtual bool update(float dt){return true;} //returns false if the manager needs to be closed

        bool getPaused(){return mPaused;}
        virtual void setPaused(bool paused){mPaused=paused;}

    protected:
        bool mPaused;

    private:
};

#endif // MANAGER_H
