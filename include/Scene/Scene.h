#ifndef SCENE_H
#define SCENE_H

#include <vector>
#include <SFML/Graphics/RenderWindow.hpp>

class SceneManager;
class GameObject;

class Scene
{
    public:
        Scene();
        virtual ~Scene();

        void initialize();
        bool update(float deltaTime);
        virtual void onRender(sf::RenderTarget *target, sf::RenderStates states = sf::RenderStates::Default);

        //void save(std::string fileName);
        //void load(std::string fileName);

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
