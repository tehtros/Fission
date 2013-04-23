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
        RenderingManager(Game *game, int width, int height);
        virtual ~RenderingManager();

        virtual bool update(float dt);

        void beginRender();
        void initializeView();
        void deinitializeView();
        void renderLights();
        void endRender();

        // Accessors
        sf::RenderWindow *getRenderWindow(){return mRenderWindow;}
        ltbl::LightSystem *getLightSystem(){return mLightSystem;}

        float getPTU(){return PTU;}
        sf::Vector2f getScreenSize(){return sf::Vector2f(mRenderWindow->getSize().x, mRenderWindow->getSize().y);}
        sf::Vector2f getCameraPosition(){return mCameraPosition;}
        float getCameraRotation(){return mCameraRotation;}
        float getCameraZoom(){return mCameraZoom;}
        sf::Vector2f screenToWorld(sf::Vector2f screenPos);
        sf::Vector2f worldToScreen(sf::Vector2f worldPos);

        // Mutators
        void setCameraPosition(sf::Vector2f pos){mCameraPosition=pos;}
        void setCameraRotation(float rot){mCameraRotation=rot;}
        void setCameraZoom(float zoom){mCameraZoom=zoom;}

    protected:
        sf::RenderWindow *mRenderWindow; //the sfml render window
        sf::View mView; // SFML view
        ltbl::LightSystem *mLightSystem; //the lighting system

        /// Pixel to unit ratio
        float mPTU;

        /// Position of the camera
        sf::Vector2f mCameraPosition;

        /// Rotation of the camera
        float mCameraRotation;

        /// Zoom of the camera
        float mCameraZoom;

    private:
};

#endif // RENDERINGMANAGER_H
