/*
State.h
Theodore DeRego
Copyright 2012

Abstract base class for all game states.
*/

#ifndef STATE_H
#define STATE_H

#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Network.hpp>

class State
{
    public:
        State();
        virtual ~State();

        virtual void initialize(){}
        virtual bool update(float dt);

        // Networking stuff
        virtual void onConnect(int ID){}
        virtual void onDisconnect(int ID){}
        virtual void handlePacket(sf::Packet &packet, int connectorID){}

        virtual void onPreRender(sf::RenderTarget *target, sf::RenderStates states = sf::RenderStates::Default){}
        virtual void onPostRender(sf::RenderTarget *target, sf::RenderStates states = sf::RenderStates::Default){}

        void kill(){mAlive=false;}

        // Accessors
        bool getAlive(){return mAlive;}

    protected:
        bool mAlive; //whether or not the state should live to see another frame

    private:
};

#endif // STATE_H
