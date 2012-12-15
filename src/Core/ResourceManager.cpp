#include "Core/ResourceManager.h"

ResourceManager *ResourceManager::Instance = NULL;

ResourceManager::ResourceManager()
{
    Instance = this;
}

ResourceManager::~ResourceManager()
{
    //dtor
}

sf::Texture *ResourceManager::getTexture(std::string name)
{
    sf::Texture *texture = NULL;
    Resource *resource = findResource(name);

    if (resource) //resource exists - use it
        texture = (sf::Texture*)resource->mData;
    else //it's a new resource - load it
    {
        //new resource
        Resource rc;
        rc.mName = name;

        //load the texture
        texture = new sf::Texture;
        texture->loadFromFile(name);
        texture->setRepeated(true); //repeat by default
        rc.mData = texture;

        //add the resource
        mResources.push_back(rc);
    }

    return texture;
}

Resource *ResourceManager::findResource(std::string name)
{
    for (unsigned int r = 0; r < mResources.size(); r++)
        if (mResources[r].mName == name)
            return &mResources[r];

    return NULL;
}
