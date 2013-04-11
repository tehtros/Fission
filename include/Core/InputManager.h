#ifndef INPUTMANAGER_H
#define INPUTMANAGER_H

#include <SFML/Window.hpp>

#include "Core/Manager.h"

namespace ButtonState
{
    enum
    {
        UP,
        DOWN,
        PRESSED,
        RELEASED
    };
}

class InputManager : public Manager
{
    public:
        struct MouseState
        {
            int mLMBState;
            int mMMBState;
            int mRMBState;
            sf::Vector2f mPosition;
            sf::Vector2f mMove;
        };

    public:
        InputManager(sf::Window *window);
        virtual ~InputManager();

        virtual bool update(float dt);

        // Accessors
        int getTime(){return mClock.getElapsedTime().asMilliseconds();}

        // Keyboard
        int getKeyState(int key){return mKeyStates[key];}
        bool getKeyDown(int key){return mKeyStates[key]==ButtonState::DOWN||mKeyStates[key]==ButtonState::PRESSED;}
        bool getKeyUp(int key){return mKeyStates[key]==ButtonState::UP||mKeyStates[key]==ButtonState::RELEASED;}

        // Mouse
        int getLMBState(){return mMouseState.mLMBState;}
        bool getLMBDown(){return mMouseState.mLMBState==ButtonState::DOWN||mMouseState.mLMBState==ButtonState::PRESSED;}
        bool getLMBUp(){return mMouseState.mLMBState==ButtonState::UP||mMouseState.mLMBState==ButtonState::RELEASED;}

        int getMMBState(){return mMouseState.mMMBState;}
        bool getMMBDown(){return mMouseState.mMMBState==ButtonState::DOWN||mMouseState.mMMBState==ButtonState::PRESSED;}
        bool getMMBUp(){return mMouseState.mMMBState==ButtonState::UP||mMouseState.mMMBState==ButtonState::RELEASED;}

        int getRMBState(){return mMouseState.mRMBState;}
        bool getRMBDown(){return mMouseState.mRMBState==ButtonState::DOWN||mMouseState.mRMBState==ButtonState::PRESSED;}
        bool getRMBUp(){return mMouseState.mRMBState==ButtonState::UP||mMouseState.mRMBState==ButtonState::RELEASED;}

        sf::Vector2f getMousePosition(){return mMouseState.mPosition;}
        sf::Vector2f getMouseMove(){return mMouseState.mMove;}

        static InputManager *get(){return Instance;}

    protected:
        /// The window this application is contained in
        sf::Window *mWindow;

        /// App clock
        sf::Clock mClock;

        /// Keyboard key states
        int mKeyStates[sf::Keyboard::KeyCount];

        /// Mouse state
        MouseState mMouseState;

    private:
        static InputManager *Instance;
};

#endif // INPUTMANAGER_H
