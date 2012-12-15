#ifndef RENDERINGMANAGER_H
#define RENDERINGMANAGER_H

#include <SFML/Graphics.hpp>
#include <LTBL/Light/LightSystem.h>
#include <LTBL/Light/Light_Point.h>
#include <LTBL/Utils.h>

#include <Core/Math.h>

class RenderingManager
{
    public:
        RenderingManager();
        virtual ~RenderingManager();

        virtual bool update(float dt);

        void beginRender();
        void endRender();

        sf::RenderWindow *getRenderWindow(){return mRenderWindow;}
        ltbl::LightSystem *getLightSystem(){return mLightSystem;}

        float getPTU(){return PTU;}

        static RenderingManager *get(){return Instance;}

    protected:
        sf::RenderWindow *mRenderWindow; //the sfml render window
        ltbl::LightSystem *mLightSystem; //the lighting system

        float mPTU; //pixels to units

    private:
        static RenderingManager *Instance;
};

#endif // RENDERINGMANAGER_H
