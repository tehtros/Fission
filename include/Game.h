#ifndef GAME_H
#define GAME_H

class State;
class ResourceManager;
class InputManager;
class StateManager;
class RenderingManager;
class PhysicsManager;
class SceneManager;
class NetworkManager;

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
        NetworkManager *mNetworkManager;

        bool mRunning;

        int mFrameRate; //the FPS that we calculate

    private:
};

#endif // GAME_H
