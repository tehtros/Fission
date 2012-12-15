/*
GameObject
Theodore DeRego
Copyright 2012

GameObject implementation
*/

#include "Core/GameObject.h"

GameObject::GameObject()
{
    mAlive = true;

    mRotation = 0;
}

GameObject::~GameObject()
{
    //dtor
}

bool GameObject::update(float dt)
{
    for (unsigned int c = 0; c < mComponents.size(); c++)
    {
        //update the components
        if (!mComponents[c]->update(dt))
        {
            //if the component doesn't want to live, end his suffering
            mComponents[c]->release();
            mComponents.erase(mComponents.begin()+c);
        }
    }

    return mAlive;
}

void GameObject::onRender(sf::RenderTarget *target, sf::RenderStates states)
{
    for (unsigned int c = 0; c < mComponents.size(); c++)
    {
        mComponents[c]->onRender(target, states);
    }
}

void GameObject::onContactBegin(GameObject *object)
{
    for (unsigned int c = 0; c < mComponents.size(); c++)
    {
        mComponents[c]->onContactBegin(object);
    }
}

void GameObject::onContactEnd(GameObject *object)
{
    for (unsigned int c = 0; c < mComponents.size(); c++)
    {
        mComponents[c]->onContactEnd(object);
    }
}

void GameObject::addComponent(Component *component)
{
    mComponents.push_back(component);
}

void GameObject::removeComponent(Component *component)
{
    for (unsigned int c = 0; c < mComponents.size(); c++)
    {
        //check if he is the one we are looking for...
        if (mComponents[c] == component)
        {
            //keeeeel it!!!!!
            mComponents[c]->release();
            mComponents.erase(mComponents.begin()+c);
        }
    }
}

void GameObject::setPosition(sf::Vector2f position, Component *caller)
{
    mPosition = position;

    for (unsigned int c = 0; c < mComponents.size(); c++)
    {
        //make sure the component who called this doesn't have his position set - that would be redundant
        if (mComponents[c] != caller)
            mComponents[c]->onSetPosition(position);
    }
}

void GameObject::setRotation(float rotation, Component *caller)
{
    mRotation = rotation;

    for (unsigned int c = 0; c < mComponents.size(); c++)
    {
        //make sure the component who called this doesn't have his position set - that would be redundant
        if (mComponents[c] != caller)
            mComponents[c]->onSetRotation(rotation);
    }
}
