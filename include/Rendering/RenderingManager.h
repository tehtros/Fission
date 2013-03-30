#ifndef RENDERINGMANAGER_H
#define RENDERINGMANAGER_H

#include <SFML/Graphics.hpp>
#include <LTBL/Light/LightSystem.h>
#include <LTBL/Light/Light_Point.h>
#include <LTBL/Utils.h>

#include <Core/Manager.h>
#include <Core/Math.h>

class RenderingManager : public Manager
{
    public:
        RenderingManager();
        virtual ~RenderingManager();

        virtual bool update(float dt);

        void beginRender();
        void renderLights();
        void endRender();

        // Accessors
        sf::RenderWindow *getRenderWindow(){return mRenderWindow;}
        ltbl::LightSystem *getLightSystem(){return mLightSystem;}

        float getPTU(){return PTU;}
        sf::Vector2f getScreenSize(){return sf::Vector2f(mRenderWindow->getSize().x, mRenderWindow->getSize().y);}
        sf::Vector2f getCameraPosition(){return mCameraPosition;}
        sf::Vector2f getCameraScreenOffset(){return (sf::Vector2f(-mCameraPosition.x, mCameraPosition.y)*mPTU)+(mRenderWindow->getView().getSize()/2.f);}

        // Mutators
        void setCameraPosition(sf::Vector2f pos){mCameraPosition=pos;}

        static RenderingManager *get(){return Instance;}

    protected:
        sf::RenderWindow *mRenderWindow; //the sfml render window
        ltbl::LightSystem *mLightSystem; //the lighting system

        /// Pixel to unit ratio
        float mPTU;

        /// Position of the camera
        sf::Vector2f mCameraPosition;

    private:
        static RenderingManager *Instance;
};

#endif // RENDERINGMANAGER_H
