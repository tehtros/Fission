#include "Game.h"

#include <Core/Math.h>
#include <Core/ResourceManager.h>
#include <Core/InputManager.h>
#include <Core/StateManager.h>
#include <Rendering/RenderingManager.h>
#include <Physics/PhysicsManager.h>
#include <Scene/SceneManager.h>
#include <GUI/GUIManager.h>
#include <Network/NetworkManager.h>

Game::Game(int width, int height)
{
    mRunning = true;

    mResourceManager = new ResourceManager;
    mStateManager = new StateManager;
    mSceneManager = new SceneManager;
    mGUIManager = new GUIManager;
    mRenderingManager = new RenderingManager(width, height);
    mPhysicsManager = new PhysicsManager;
    mInputManager = new InputManager(mRenderingManager->getRenderWindow());
    mNetworkManager = new NetworkManager;

    mLockStep = 1.f/30.f;
}

Game::~Game()
{
    delete mResourceManager;
    delete mStateManager;
    delete mSceneManager;
    delete mGUIManager;
    delete mRenderingManager;
    delete mPhysicsManager;
    delete mInputManager;
    delete mNetworkManager;
}

void Game::run(State *state)
{
    mStateManager->pushState(state);

    int lastFrameTime = InputManager::get()->getTime();
    float deltaTime = 0;
    mLockStepAccumulator = 0.f;
    mLockStepAccumulatorRatio = 0.f;

    while (mRunning)
    {
        while (InputManager::get()->getTime()-lastFrameTime < 1); // Cap framerate at 1000 FPS

        deltaTime = (InputManager::get()->getTime()-lastFrameTime)/1000.f;
        lastFrameTime = InputManager::get()->getTime();

        //calculate framerate
        mFrameRate = 1.f/deltaTime;

        // Calculate fixed time step accumulator stuff
        mLockStepAccumulator += deltaTime; //add the frame time to the accumulator
        int physicsSteps = floorf(mLockStepAccumulator / mLockStep); //calculate physics steps needed this frame

        if (physicsSteps > 0) //insure that there are no rounding errors
            mLockStepAccumulator -= physicsSteps * mLockStep;

        mLockStepAccumulatorRatio = mLockStepAccumulator / mLockStep;

        for (int s = 0; s < physicsSteps; s++)
        {
            float timeStep = mLockStep;

            // Update the managers
            if (mRunning && !mPhysicsManager->getPaused())
                mRunning = mPhysicsManager->update(timeStep);
            if (mRunning && !mInputManager->getPaused())
                mRunning = mInputManager->update(timeStep);
            if (mRunning && !mGUIManager->getPaused())
                mRunning = mGUIManager->update(timeStep);
            if (mRunning && !mSceneManager->getPaused())
                mRunning = mSceneManager->update(timeStep);
            if (mRunning && !mStateManager->getPaused())
                mRunning = mStateManager->update(timeStep);
            if (mRunning && !mRenderingManager->getPaused())
                mRunning = mRenderingManager->update(timeStep);
        }

        // Update network manager disregarding lockstep
        if (mRunning && !mNetworkManager->getPaused())
            mRunning = mNetworkManager->update(deltaTime);

        if (!mRunning)
            break;

        //start rendering
        mRenderingManager->beginRender();

        mStateManager->getCurrentState()->onPreRender(mRenderingManager->getRenderWindow());

        mRenderingManager->initializeView();

        mSceneManager->getCurrentScene()->render(mRenderingManager->getRenderWindow());

        mRenderingManager->renderLights();

        mRenderingManager->deinitializeView();

        mStateManager->getCurrentState()->onPostRender(mRenderingManager->getRenderWindow());

        mGUIManager->render(mRenderingManager->getRenderWindow());

        //finish rendering
        mRenderingManager->endRender();
    }
}
