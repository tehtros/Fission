/*
State.cpp
Theodore DeRego
Copyright 2012

State implementation
*/

#include "Core/State.h"

State::State(Game *game) : GameRef(game)
{
    mAlive = true;
}

State::~State()
{
    //dtor
}

bool State::update(float dt)
{
    return mAlive;
}
