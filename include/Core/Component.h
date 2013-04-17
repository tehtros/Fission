/*
Component.h
Theodore DeRego
Copyright 2012

Defines a component of a GameObject. Derive from this to add functionality to a GameObject
*/

#ifndef COMPONENT_H
#define COMPONENT_H

#include <string>

#include <Box2D/Dynamics/Contacts/b2Contact.h>
#include <SFML/System/Vector2.hpp>
#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Network/Packet.hpp>

#include "Core/RefCounted.h"

class GameObject;

class Component : public RefCounted
{
    public:
        Component(GameObject *object, std::string name);
        virtual ~Component();

        virtual void serialize(sf::Packet &packet);
        virtual void deserialize(sf::Packet &packet);

        virtual bool update(float dt){return true;}
        virtual void onRender(sf::RenderTarget *target, sf::RenderStates states = sf::RenderStates::Default){}

        /// Handle packets sent to this Component
        virtual void handlePacket(sf::Packet &packet){}

        virtual void onSetPosition(sf::Vector2f position){} //called when the object's position is set
        virtual void onSetRotation(float rotation){} //called when the object's position is set

        /// Called before a collision with the owning object is processed
        virtual void onPreSolve(GameObject *object, b2Contact* contact, const b2Manifold* oldManifold){}

        /// Called when the owning GameObject starts colliding with another
        virtual void onContactBegin(GameObject *object){}

        /// Called when a collision ends
        virtual void onContactEnd(GameObject *object){}

        //accessors
        GameObject *getGameObject(){return mGameObject;}
        std::string getName(){return mName;}
        std::string getTypeName(){return mTypeName;}
        bool getShouldSerialize(){return mShouldSerialize;}
        bool getEnabled(){return mEnabled;}

        //mutators
        void setName(std::string name){mName=name;}
        void getTypeName(std::string typeName){mTypeName=typeName;}
        void setShouldSerialize(bool serialize){mShouldSerialize=serialize;}
        virtual void setEnabled(bool enabled){mEnabled=enabled;}

    protected:
        GameObject *mGameObject;
        std::string mName;
        std::string mTypeName;
        bool mShouldSerialize;

        bool mEnabled;

    private:
};

#endif // COMPONENT_H
