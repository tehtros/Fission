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
}

Component::~Component()
{
    //dtor
}
