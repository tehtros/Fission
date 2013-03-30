#ifndef SCENEMANAGER_H
#define SCENEMANAGER_H

#include <string>
#include <vector>

#include "Core/Manager.h"

#include "Scene.h"

class Component;

typedef Component *(*ComponentCreationFunction)(GameObject *);

class SceneManager : public Manager
{
    public:
        SceneManager();
        virtual ~SceneManager();

        virtual bool update(float dt);

        //void saveScene(std::string fileName){mCurrentScene->save(fileName);}
        //void loadScene(std::string fileName){mCurrentScene->load(fileName);}

        GameObject *createGameObject();
        void addGameObject(GameObject *object){mCurrentScene->addGameObject(object);}
        void destroyGameObject(GameObject *object){mCurrentScene->destroyGameObject(object);}
        GameObject *findGameObject(int ID){return mCurrentScene->findGameObject(ID);}

        void clearScene(){mCurrentScene->clear();}

        void registerComponentCreationFunction(std::string name, ComponentCreationFunction funcPointer);
        ComponentCreationFunction getComponentCreationFunction(std::string name);
        void removeComponentCreationFunction(std::string name);

        void saveGameObject(GameObject *object, std::string fileName);
        GameObject *loadGameObject(std::string fileName);

        // Accessors
        Scene *getCurrentScene(){return mCurrentScene;}

        // Mutators

        static SceneManager *get(){return Instance;}

    protected:
        /// List of all the loaded scenes
        std::vector <Scene*> mScenes;

        /// The scene currently being used
        Scene *mCurrentScene;

        std::vector <ComponentCreationFunction> mComponentCreationFunctions;
        std::vector <std::string> mComponentCreationFunctionNames;

    private:
        static SceneManager *Instance;
};

#endif // SCENEMANAGER_H
