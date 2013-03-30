#include "Scene/SceneManager.h"

#include <fstream>

#include "Scene/SceneManager.h"
#include "Scene/Scene.h"
#include "Core/GameObject.h"

#include "Rendering/SpriteComponent.h"
#include "Physics/RigidBodyComponent.h"
#include "Physics/DragComponent.h"
#include "Logic/WeaponComponent.h"

SceneManager *SceneManager::Instance = NULL;

SceneManager::SceneManager()
{
    Instance = this;

    mCurrentScene = new Scene;
    mScenes.push_back(mCurrentScene);

    registerComponentCreationFunction("SpriteComponent", SpriteComponent::createComponent);
    registerComponentCreationFunction("RigidBodyComponent", RigidBodyComponent::createComponent);
    registerComponentCreationFunction("DragComponent", DragComponent::createComponent);
    registerComponentCreationFunction("WeaponComponent", WeaponComponent::createComponent);
}

SceneManager::~SceneManager()
{
    for (unsigned int s = 0; s < mScenes.size(); s++)
        delete mScenes[s];
}

bool SceneManager::update(float deltaTime)
{
    return mCurrentScene->update(deltaTime);
}

GameObject *SceneManager::createGameObject()
{
    GameObject *object = new GameObject;
    addGameObject(object);
    return object;
}

void SceneManager::registerComponentCreationFunction(std::string name, ComponentCreationFunction funcPointer)
{
    mComponentCreationFunctionNames.push_back(name);
    mComponentCreationFunctions.push_back(funcPointer);
}

void SceneManager::removeComponentCreationFunction(std::string name)
{
    for (unsigned int f = 0; f < mComponentCreationFunctions.size(); f++)
    {
        if (name == mComponentCreationFunctionNames[f])
        {
            mComponentCreationFunctions.erase(mComponentCreationFunctions.begin()+f);
            //mComponentCreationFunctionNames.erase(f);
            return;
        }
    }
}

ComponentCreationFunction SceneManager::getComponentCreationFunction(std::string name)
{
    for (unsigned int f = 0; f < mComponentCreationFunctions.size(); f++)
    {
        if (name == mComponentCreationFunctionNames[f])
            return mComponentCreationFunctions[f];
    }

    return NULL;
}

void SceneManager::saveGameObject(GameObject *object, std::string fileName)
{
    std::ofstream file(fileName.c_str(), std::ios::out|std::ios::binary);

    sf::Packet packet;
    object->serialize(packet);

    file.write((const char*)packet.getData(), packet.getDataSize());

    file.close();
}

GameObject *SceneManager::loadGameObject(std::string fileName)
{
    std::ifstream file(fileName.c_str(), std::ios::in|std::ios::binary);

    GameObject *object = createGameObject();
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

    object->deserialize(packet);

    packet.clear();
    file.close();

    return object;
}
