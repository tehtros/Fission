#include "Game.h"

#include <Core/Math.h>
#include <Core/ResourceManager.h>
#include <Core/InputManager.h>
#include <Core/StateManager.h>
#include <Rendering/RenderingManager.h>
#include <Physics/PhysicsManager.h>
#include <Scene/SceneManager.h>
#include <Network/NetworkManager.h>

Game::Game()
{
    mRunning = true;

    mResourceManager = new ResourceManager;
    mStateManager = new StateManager;
    mSceneManager = new SceneManager;
    mRenderingManager = new RenderingManager;
    mPhysicsManager = new PhysicsManager;
    mInputManager = new InputManager(mRenderingManager->getRenderWindow());
    mNetworkManager = new NetworkManager;
}

Game::~Game()
{
    //deltaTimeor
}

void Game::run(State *state)
{
    mStateManager->pushState(state);

    int lastFrameTime = InputManager::get()->getTime();
    float deltaTime = 0;

    while (mRunning)
    {
        deltaTime = (InputManager::get()->getTime()-lastFrameTime)/1000.f;
        lastFrameTime = InputManager::get()->getTime();

        //calculate framerate
        mFrameRate = 1.f/deltaTime;

        if (mRunning && !mPhysicsManager->getPaused())
            mRunning = mPhysicsManager->update(deltaTime);
        if (mRunning && !mInputManager->getPaused())
            mRunning = mInputManager->update(deltaTime);
        if (mRunning && !mNetworkManager->getPaused())
            mRunning = mNetworkManager->update(deltaTime);
        if (mRunning && !mSceneManager->getPaused())
            mRunning = mSceneManager->update(deltaTime);
        if (mRunning && !mStateManager->getPaused())
            mRunning = mStateManager->update(deltaTime);
        if (mRunning && !mRenderingManager->getPaused())
            mRunning = mRenderingManager->update(deltaTime);

        //start rendering
        mRenderingManager->beginRender();

        mStateManager->getCurrentState()->onPreRender(mRenderingManager->getRenderWindow());
        mSceneManager->getCurrentScene()->onRender(mRenderingManager->getRenderWindow());

        mRenderingManager->renderLights();

        mStateManager->getCurrentState()->onPostRender(mRenderingManager->getRenderWindow());

        //finish rendering
        mRenderingManager->endRender();
    }
}
