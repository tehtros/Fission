#ifndef SCENE_H
#define SCENE_H

#include <vector>
#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Network/Packet.hpp>

#include "Core/GameRef.h"

class SceneManager;
class GameObject;

class Scene : public GameRef
{
    public:
        Scene(Game *game);
        virtual ~Scene();

        void save(std::string filename);
        void load(std::string filename);
        bool update(float deltaTime);
        void render(sf::RenderTarget *target, sf::RenderStates states = sf::RenderStates::Default);

        //void save(std::string fileName);
        //void load(std::string fileName);

        void serialize(sf::Packet &packet);
        void deserialize(sf::Packet &packet);

        void serializeUpdatePacket(sf::Packet &packet);

        void addGameObject(GameObject *object);
        void destroyGameObject(GameObject *object);
        GameObject *findGameObject(int ID);

        void clear();

        std::vector <GameObject*> &getGameObjects(){return mGameObjects;}

    protected:
        std::vector <GameObject*> mGameObjects;

    private:
        friend class SceneManager;
};

#endif // SCENE_H
