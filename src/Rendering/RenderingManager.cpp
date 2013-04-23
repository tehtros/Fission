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
    mCameraZoom = 1.f;
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
    sf::Vector2f halfScreen = sf::Vector2f(mRenderWindow->getSize().x, mRenderWindow->getSize().y)/2.f;
    mView.move(sf::Vector2f(mCameraPosition.x, -mCameraPosition.y)*mPTU - halfScreen);
    mView.setRotation(-mCameraRotation);
    mView.zoom(mCameraZoom);
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
    sf::View view = mRenderWindow->getDefaultView();
    sf::Vector2f halfScreen = sf::Vector2f(mRenderWindow->getSize().x, mRenderWindow->getSize().y)/2.f;
    view.move(sf::Vector2f(mCameraPosition.x, -mCameraPosition.y)*mPTU - halfScreen);
    view.setRotation(-mCameraRotation);
    view.zoom(mCameraZoom);

    sf::Vector2f worldPos = mRenderWindow->mapPixelToCoords(sf::Vector2i(screenPos.x, screenPos.y), view);
    worldPos.y *= -1;
    worldPos /= mPTU;

    return worldPos;
}

sf::Vector2f RenderingManager::worldToScreen(sf::Vector2f worldPos)
{
    sf::View view = mRenderWindow->getDefaultView();
    sf::Vector2f halfScreen = sf::Vector2f(mRenderWindow->getSize().x, mRenderWindow->getSize().y)/2.f;
    view.move(sf::Vector2f(mCameraPosition.x, -mCameraPosition.y)*mPTU - halfScreen);
    view.setRotation(-mCameraRotation);
    view.zoom(mCameraZoom);

    worldPos.y *= -1;
    worldPos *= mPTU;
    sf::Vector2i screenPos = mRenderWindow->mapCoordsToPixel(sf::Vector2f(worldPos.x, worldPos.y), view);

    return sf::Vector2f(screenPos.x, screenPos.y);
}
