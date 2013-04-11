#ifndef GAME_H
#define GAME_H

class State;
class ResourceManager;
class InputManager;
class StateManager;
class RenderingManager;
class PhysicsManager;
class SceneManager;
class GUIManager;
class NetworkManager;

class Game
{
    public:
        Game(int width, int height);
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
        GUIManager *mGUIManager;
        NetworkManager *mNetworkManager;

        bool mRunning;

        int mFrameRate; //the FPS that we calculate

        float mLockStep; // The tick rate of the physics engine
        float mLockStepAccumulator; // Accumulator for physics time processing
        float mLockStepAccumulatorRatio; // Ratio of delta time left to physics lock step

    private:
};

#endif // GAME_H
