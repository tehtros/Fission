#include "Rendering/RenderingManager.h"

RenderingManager *RenderingManager::Instance = NULL;

RenderingManager::RenderingManager()
{
    Instance = this;

    // Create the window
    mRenderWindow = new sf::RenderWindow(sf::VideoMode(800,600,32), "Test");

    // Create the light system
    mLightSystem = new ltbl::LightSystem(AABB(Vec2f(0.0f, 0.0f), Vec2f(mRenderWindow->getSize().x, mRenderWindow->getSize().y)),
                                         mRenderWindow, "Content/Textures/lightFin.png", "Content/Shaders/lightAttenuationShader.frag");
    mLightSystem->m_useBloom = true;

    mPTU = 32.f; //32 pixels per unit
}

RenderingManager::~RenderingManager()
{
    mRenderWindow->close();
    delete mLightSystem;
    delete mRenderWindow;
}

bool RenderingManager::update(float dt)
{
    return mRenderWindow->isOpen();
}

void RenderingManager::beginRender()
{
    mRenderWindow->clear(); //clear the window

    mLightSystem->SetView(mRenderWindow->getView());
}

void RenderingManager::endRender()
{
    //calculate the lights
    mLightSystem->RenderLights();

    //draw the lights
    mLightSystem->RenderLightTexture();

    mRenderWindow->display(); //display the window
}
