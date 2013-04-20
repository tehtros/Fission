#include "Core/ResourceManager.h"

ResourceManager::ResourceManager(Game *game) : Manager(game)
{
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

sf::Font *ResourceManager::getFont(std::string name)
{
    sf::Font *font = NULL;
    Resource *resource = findResource(name);

    if (resource) //resource exists - use it
        font = (sf::Font*)resource->mData;
    else //it's a new resource - load it
    {
        //new resource
        Resource rc;
        rc.mName = name;

        //load the font
        font = new sf::Font;
        font->loadFromFile(name);
        rc.mData = font;

        //add the resource
        mResources.push_back(rc);
    }

    return font;
}

Resource *ResourceManager::findResource(std::string name)
{
    for (unsigned int r = 0; r < mResources.size(); r++)
        if (mResources[r].mName == name)
            return &mResources[r];

    return NULL;
}
