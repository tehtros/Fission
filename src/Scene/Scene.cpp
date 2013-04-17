#include "Scene/Scene.h"

#include <fstream>

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

void Scene::save(std::string fileName)
{
    std::ofstream file(fileName.c_str(), std::ios::out|std::ios::binary);

    sf::Packet packet;
    serialize(packet);

    file.write((const char*)packet.getData(), packet.getDataSize());

    file.close();
}

void Scene::load(std::string fileName)
{
    std::ifstream file(fileName.c_str(), std::ios::in|std::ios::binary);

    sf::Packet packet;

    int size = file.tellg();
    file.seekg(0, std::ios::end);
    size = ((int)file.tellg()) - size;
    file.seekg(0, std::ios::beg);

    // Create the buffer and read file contents into it
    char *buffer = new char[size];
    file.read(buffer, size);
    packet.append(buffer, size);
    delete buffer;

    deserialize(packet);

    packet.clear();
    file.close();
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

void Scene::render(sf::RenderTarget *target, sf::RenderStates states)
{
    int topLayer = 0;
    for (unsigned int o = 0; o < mGameObjects.size(); o++)
        topLayer = std::max(topLayer, mGameObjects[o]->getRenderLayer());

    for (int l = 0; l <= topLayer; l++)
    {
        for (unsigned int o = 0; o < mGameObjects.size(); o++)
        {
            if (mGameObjects[o]->getRenderLayer() == l)
                mGameObjects[o]->onRender(target, states);
        }
    }
}

void Scene::serialize(sf::Packet &packet)
{
    packet << (int)mGameObjects.size();
    for (unsigned int o = 0; o < mGameObjects.size(); o++)
    {
        mGameObjects[o]->serialize(packet);
    }
}

void Scene::deserialize(sf::Packet &packet)
{
    int objectCount;
    packet >> objectCount;

    for (int o = 0; o < objectCount; o++)
    {
        GameObject *object = SceneManager::get()->createGameObject();
        object->deserialize(packet);

        if (object->getID() != -1 && object != findGameObject(object->getID()))
        {
            object->kill();
        }
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
    if (ID == -1)
        return NULL;

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
