#ifndef RESOURCEMANAGER_H
#define RESOURCEMANAGER_H

#include <string>
#include <vector>
#include <SFML/Graphics/Font.hpp>
#include <SFML/Graphics/Texture.hpp>

#include <Core/Manager.h>

struct Resource
{
    std::string mName;
    void *mData;
};

class ResourceManager : public Manager
{
    public:
        ResourceManager(Game *game);
        virtual ~ResourceManager();

        sf::Texture *getTexture(std::string name);
        sf::Font *getFont(std::string name);

    protected:
        Resource *findResource(std::string name);

        //all of the resources
        std::vector <Resource> mResources;

    private:
};

#endif // RESOURCEMANAGER_H
