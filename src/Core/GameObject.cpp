/*
GameObject
Theodore DeRego
Copyright 2012

GameObject implementation
*/

#include "Core/GameObject.h"

#include <Scene/SceneManager.h>

GameObject::GameObject()
{
    mAlive = true;

    mSyncNetwork = false; // By default, don't sync over the network

    mRotation = 0;
}

GameObject::~GameObject()
{
    for (unsigned int c = 0; c < mComponents.size(); c++)
    {
        Component *component = mComponents[c];
        mComponents.erase(mComponents.begin()+c);
        c--;
        component->release();
    }
}

void GameObject::serialize(sf::Packet &packet)
{
    packet << mID;

    int componentCount = 0; // Get number of serializeable components
    for (unsigned int c = 0; c < mComponents.size(); c++)
    {
        if (mComponents[c]->getShouldSerialize())
            componentCount++;
    }

    packet << componentCount;
    for (unsigned int c = 0; c < mComponents.size(); c++)
    {
        if (mComponents[c]->getShouldSerialize())
        {
            packet << mComponents[c]->getTypeName();
            mComponents[c]->serialize(packet);
        }
    }
}

void GameObject::deserialize(sf::Packet &packet)
{
    packet >> mID;

    // Get all the components
    int componentCount;
    std::string componentType;

    packet >> componentCount;
    for (int c = 0; c < componentCount; c++)
    {
        packet >> componentType;

        if (SceneManager::get()->getComponentCreationFunction(componentType) != NULL)
        {
            Component *component = SceneManager::get()->getComponentCreationFunction(componentType)(this);
            component->deserialize(packet);
            addComponent(component);
        }
        else
        {
            // TODO: Failed to deserialize GameObject
            return;
        }
    }
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
            c--;
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

void GameObject::onPreSolve(GameObject *object, b2Contact* contact, const b2Manifold* oldManifold)
{
    for (unsigned int c = 0; c < mComponents.size(); c++)
    {
        mComponents[c]->onPreSolve(object, contact, oldManifold);
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

Component *GameObject::addComponent(Component *component)
{
    mComponents.push_back(component);
    return component;
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
            c--;
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
