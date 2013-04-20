/*
Component.cpp
Theodore DeRego
Copyright 2012

Component implementation
*/

#include "Core/Component.h"

#include "Core/GameObject.h"

Component::Component(GameObject *object, std::string name) : GameRef(object->getGame())
{
    mGameObject = object;
    mName = name;
    mTypeName = "Component";

    mShouldSerialize = true;
    mEnabled = true;
}

Component::~Component()
{
    //dtor
}

void Component::serialize(sf::Packet &packet)
{
    packet << mName;
}

void Component::deserialize(sf::Packet &packet)
{
    packet >> mName;
}
