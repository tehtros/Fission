/*
State.h
Theodore DeRego
Copyright 2012

Abstract base class for all game states.
*/

#ifndef STATE_H
#define STATE_H

#include <SFML/Graphics/RenderWindow.hpp>

class State
{
    public:
        State();
        virtual ~State();

        virtual void initialize(){}
        virtual bool update(float dt);

        virtual void onPreRender(sf::RenderTarget *target, sf::RenderStates states = sf::RenderStates::Default){}
        virtual void onPostRender(sf::RenderTarget *target, sf::RenderStates states = sf::RenderStates::Default){}

        void kill(){mAlive=false;}

        //accessors
        bool getAlive(){return mAlive;}

    protected:
        bool mAlive; //whether or not the object should live to see another frame

    private:
};

#endif // STATE_H
