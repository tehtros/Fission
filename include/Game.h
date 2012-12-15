#ifndef GAME_H
#define GAME_H

#include <Core/ResourceManager.h>
#include <Core/InputManager.h>
#include <Core/StateManager.h>
#include <Rendering/RenderingManager.h>
#include <Physics/PhysicsManager.h>
#include <Scene/SceneManager.h>

class State;

class Game
{
    public:
        Game();
        virtual ~Game();

        virtual void run(State *state);

        void close(){mRunning=false;}

        //accessors
        int getFrameRate(){return mFrameRate;}

    protected:
        ResourceManager *mResourceManager;
        InputManager *mInputManager;
        StateManager *mStateManager;
        RenderingManager *mRenderingManager;
        PhysicsManager *mPhysicsManager;
        SceneManager *mSceneManager;

        bool mRunning;

        int mFrameRate; //the FPS that we calculate

    private:
};

#endif // GAME_H
