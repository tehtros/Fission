#include "Scene/Scene.h"

#include "Core/GameObject.h"
#include "Rendering/RenderingManager.h"
#include "Scene/SceneManager.h"


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

void Scene::addGameObject(GameObject *object)
{
    mGameObjects.push_back(object);
}


void Scene::destroyGameObject(GameObject *object)
{
    for (unsigned int o = 0; o < mGameObjects.size(); o++)
    {
        if (mGameObjects[o] == object) //find the GameObject
        {
            mGameObjects[o]->release(); //release the GameObject
            mGameObjects.erase(mGameObjects.begin()+o);
            return;
        }
    }
}

void Scene::clear()
{
    for (unsigned int o = 0; o < mGameObjects.size(); o++)
        mGameObjects[o]->release(); //release the GameObject
    mGameObjects.clear();
}
