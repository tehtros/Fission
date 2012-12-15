#include "Core/InputManager.h"

#include <iostream>

InputManager *InputManager::Instance = NULL;

InputManager::InputManager(sf::Window *window)
{
    Instance = this;

    mWindow = window;

    // All the keys are up by default
    for (int k = 0; k < sf::Keyboard::KeyCount; k++)
        mKeyStates[k] = ButtonState::UP;

    mMouseState.mLMBState = ButtonState::UP;
    mMouseState.mMMBState = ButtonState::UP;
    mMouseState.mRMBState = ButtonState::UP;
}

InputManager::~InputManager()
{
    //dtor
}

bool InputManager::update(float dt)
{
    // Update pressed and released keystates to be down or up
    for (int k = 0; k < sf::Keyboard::KeyCount; k++)
    {
        if (mKeyStates[k] == ButtonState::PRESSED)
            mKeyStates[k] = ButtonState::DOWN;
        else if (mKeyStates[k] == ButtonState::RELEASED)
            mKeyStates[k] = ButtonState::UP;
    }

    // Do the same for the mouse buttons

    if (mMouseState.mLMBState == ButtonState::PRESSED)
            mMouseState.mLMBState = ButtonState::DOWN;
    else if (mMouseState.mLMBState == ButtonState::RELEASED)
        mMouseState.mLMBState = ButtonState::UP;

    if (mMouseState.mMMBState == ButtonState::PRESSED)
            mMouseState.mMMBState = ButtonState::DOWN;
    else if (mMouseState.mMMBState == ButtonState::RELEASED)
        mMouseState.mMMBState = ButtonState::UP;

    if (mMouseState.mRMBState == ButtonState::PRESSED)
            mMouseState.mRMBState = ButtonState::DOWN;
    else if (mMouseState.mRMBState == ButtonState::RELEASED)
        mMouseState.mRMBState = ButtonState::UP;

    // Handle new events
    sf::Event event;

    //while there are pending events...
    while (mWindow->pollEvent(event))
    {
        //check the type of the event...
        switch (event.type)
        {
            // window closed
            case sf::Event::Closed:
                mWindow->close();
                break;

            // key pressed
            case sf::Event::KeyPressed:
            {
                mKeyStates[event.key.code] = ButtonState::PRESSED;
                break;
            }

            // key released
            case sf::Event::KeyReleased:
            {
                mKeyStates[event.key.code] = ButtonState::RELEASED;
                break;
            }

            // Mouse button pressed
            case sf::Event::MouseButtonPressed:
            {
                switch (event.mouseButton.button)
                {
                    case sf::Mouse::Button::Left:
                        mMouseState.mLMBState = ButtonState::PRESSED;
                        break;
                    case sf::Mouse::Button::Middle:
                        mMouseState.mMMBState = ButtonState::PRESSED;
                        break;
                    case sf::Mouse::Button::Right:
                        mMouseState.mRMBState = ButtonState::PRESSED;
                        break;
                    default: break;
                }
                break;
            }

            // Mouse button released
            case sf::Event::MouseButtonReleased:
            {
                switch (event.mouseButton.button)
                {
                    case sf::Mouse::Button::Left:
                        mMouseState.mLMBState = ButtonState::RELEASED;
                        break;
                    case sf::Mouse::Button::Middle:
                        mMouseState.mMMBState = ButtonState::RELEASED;
                        break;
                    case sf::Mouse::Button::Right:
                        mMouseState.mRMBState = ButtonState::RELEASED;
                        break;
                    default: break;
                }
                break;
            }

            // Mouse button pressed
            case sf::Event::MouseMoved:
            {
                mMouseState.mMove.x = event.mouseMove.x-mMouseState.mPosition.x;
                mMouseState.mMove.y = event.mouseMove.x-mMouseState.mPosition.y;
                mMouseState.mPosition.x = event.mouseMove.x;
                mMouseState.mPosition.y = event.mouseMove.y;
                break;
            }

            //default is to do NOTHING
            default:
                break;
        }
    }

    return true;
}
