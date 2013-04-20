#include "Rendering/RenderingManager.h"

RenderingManager::RenderingManager(Game *game, int width, int height) : Manager(game)
{
    // Create the window
    mRenderWindow = new sf::RenderWindow(sf::VideoMode(width,height,32), "EvoScroller");
    mView = mRenderWindow->getView();

    // Create the light system
    mLightSystem = new ltbl::LightSystem(AABB(Vec2f(0.0f, 0.0f), Vec2f(mRenderWindow->getSize().x, mRenderWindow->getSize().y)),
                                         mRenderWindow, "Content/Textures/lightFin.png", "Content/Shaders/lightAttenuationShader.frag");
    mLightSystem->m_useBloom = true;

    mPTU = 32.f; //32 pixels per unit
    mCameraRotation = 0.f;
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
}

void RenderingManager::initializeView()
{
    mView.setRotation(-mCameraRotation);
    mRenderWindow->setView(mView);

    mLightSystem->SetView(mRenderWindow->getView());
}

void RenderingManager::deinitializeView()
{
    mView = mRenderWindow->getDefaultView();
    mRenderWindow->setView(mView);

    mLightSystem->SetView(mRenderWindow->getView());
}

void RenderingManager::renderLights()
{
    //calculate the lights
    mLightSystem->RenderLights();

    //draw the lights
    mLightSystem->RenderLightTexture();
}

void RenderingManager::endRender()
{
    mRenderWindow->display(); //display the window
}

sf::Vector2f RenderingManager::screenToWorld(sf::Vector2f screenPos)
{
    sf::Vector2f worldPos = screenPos-getCameraScreenOffset();
    worldPos.x = worldPos.x/mPTU;
    worldPos.y = -worldPos.y/mPTU;
    worldPos.rotateBy(getGame()->getRenderingManager()->getCameraRotation(), getGame()->getRenderingManager()->getCameraPosition());

    return worldPos;
}

sf::Vector2f RenderingManager::worldToScreen(sf::Vector2f worldPos)
{
    sf::Vector2f screenPos = worldPos;
    screenPos.rotateBy(-getGame()->getRenderingManager()->getCameraRotation(), getGame()->getRenderingManager()->getCameraPosition());
    screenPos.x *= mPTU;
    screenPos.y *= -mPTU;
    screenPos += getCameraScreenOffset();

    return screenPos;
}
