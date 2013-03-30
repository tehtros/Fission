/*
Component.cpp
Theodore DeRego
Copyright 2012

Component implementation
*/

#include "Core/Component.h"

Component::Component(GameObject *object, std::string name)
{
    mGameObject = object;
    mName = name;
    mTypeName = "Component";

    mShouldSerialize = true;
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
