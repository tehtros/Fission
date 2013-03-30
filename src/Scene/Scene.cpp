#include "Scene/Scene.h"

#include "Core/GameObject.h"
#include "Rendering/RenderingManager.h"
#include "Scene/SceneManager.h"
#include "Physics/RigidBodyComponent.h"

Scene::Scene()
{
    //ctor
}

Scene::~Scene()
{
    //clear the scene
    clear();
}

void Scene::initialize()
{
}

bool Scene::update(float deltaTime)
{
    for (unsigned int o = 0; o < mGameObjects.size(); o++)
    {
        if (!mGameObjects[o]->getAlive() || !mGameObjects[o]->update(deltaTime)) //update the game object
        {
            mGameObjects[o]->release(); //release the GameObject
            mGameObjects.erase(mGameObjects.begin()+o);
            o--; //don't skip an object
        }
    }

    return true;
}

void Scene::onRender(sf::RenderTarget *target, sf::RenderStates states)
{
    for (unsigned int o = 0; o < mGameObjects.size(); o++)
    {
        mGameObjects[o]->onRender(target, states);
    }
}

void Scene::serializeCreationPacket(sf::Packet &packet)
{
    // Get networked object count
    int objectCount = 0;
    for (unsigned int o = 0; o < mGameObjects.size(); o++)
    {
        if (mGameObjects[o]->getSyncNetwork())
            objectCount++;
    }

    packet << objectCount;
    for (unsigned int o = 0; o < mGameObjects.size(); o++)
    {
        if (mGameObjects[o]->getSyncNetwork())
            mGameObjects[o]->serialize(packet);
    }
}

void Scene::deserializeCreationPacket(sf::Packet &packet)
{
    int objectCount;
    packet >> objectCount;

    for (int o = 0; o < objectCount; o++)
    {
        SceneManager::get()->createGameObject()->deserialize(packet);
    }
}

void Scene::serializeUpdatePacket(sf::Packet &packet)
{
    // Get networked object count
    int objectCount = 0;
    for (unsigned int o = 0; o < mGameObjects.size(); o++)
    {
        if (mGameObjects[o]->getSyncNetwork())
            objectCount++;
    }

    packet << objectCount;
    for (unsigned int o = 0; o < mGameObjects.size(); o++)
    {
        if (mGameObjects[o]->getSyncNetwork())
        {
            RigidBodyComponent *body = mGameObjects[o]->getComponent<RigidBodyComponent>();

            packet << mGameObjects[o]->getID();
            packet << mGameObjects[o]->getPosition().x << mGameObjects[o]->getPosition().y << mGameObjects[o]->getRotation();

            if (body)
                packet << body->getBody()->GetLinearVelocity().x << body->getBody()->GetLinearVelocity().y
                       << body->getBody()->GetAngularVelocity();
        }
    }
}

void Scene::addGameObject(GameObject *object)
{
    mGameObjects.push_back(object);
}


void Scene::destroyGameObject(GameObject *object)
{
    for (unsigned int o = 0; o < mGameObjects.size(); o++)
    {
        if (mGameObjects[o] == object) // Find the GameObject
        {
            mGameObjects[o]->kill(); // Make sure it's dead
            mGameObjects[o]->release(); // Release the GameObject
            mGameObjects.erase(mGameObjects.begin()+o);
            return;
        }
    }
}

GameObject *Scene::findGameObject(int ID)
{
    for (unsigned int o = 0; o < mGameObjects.size(); o++)
    {
        if (mGameObjects[o]->getID() == ID) //find the GameObject
        {
            return mGameObjects[o];
        }
    }

    return NULL; // Couldn't find it
}

void Scene::clear()
{
    for (unsigned int o = 0; o < mGameObjects.size(); o++)
        mGameObjects[o]->release(); //release the GameObject
    mGameObjects.clear();
}
