#include "Game.h"

#include <Core/Math.h>

Game::Game()
{
    mRunning = true;

    mResourceManager = new ResourceManager;
    mStateManager = new StateManager;
    mSceneManager = new SceneManager;
    mRenderingManager = new RenderingManager;
    mPhysicsManager = new PhysicsManager;
    mInputManager = new InputManager(mRenderingManager->getRenderWindow());
}

Game::~Game()
{
    //deltaTimeor
}

void Game::run(State *state)
{
    state->initialize();
    mStateManager->pushState(state);

    int lastFrameTime = InputManager::get()->getTime();
    float deltaTime = 0;

    while (mRunning)
    {
        deltaTime = (InputManager::get()->getTime()-lastFrameTime)/1000.f;
        lastFrameTime = InputManager::get()->getTime();

        //calculate framerate
        mFrameRate = 1.f/deltaTime;

        if (mRunning)
            mRunning = mPhysicsManager->update(deltaTime);
        if (mRunning)
            mRunning = mInputManager->update(deltaTime);
        if (mRunning)
            mRunning = mSceneManager->update(deltaTime);
        if (mRunning)
            mRunning = mStateManager->update(deltaTime);
        if (mRunning)
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
