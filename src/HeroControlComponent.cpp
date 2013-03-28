#include "HeroControlComponent.h"

#include <Core/InputManager.h>
#include <Core/GameObject.h>
#include <Network/NetworkManager.h>
#include <Rendering/SpriteComponent.h>
#include <Physics/RigidBodyComponent.h>

#include "GameState.h"

enum
{
    KEYBOARD,
    MOUSE
};

HeroControlComponent::HeroControlComponent(GameObject *object, std::string name, int networkID) : Component(object, name)
{
    for (int k = 0; k < sf::Keyboard::KeyCount; k++)
    {
        mKeyStates[k] = ButtonState::UP;
    }

    mNetworkID = networkID;

    mTypeName = "HeroControlComponent";
}

HeroControlComponent::~HeroControlComponent()
{
    //dtor
}

void HeroControlComponent::serialize(sf::Packet &packet)
{
    Component::serialize(packet);

    packet << mNetworkID;
}

void HeroControlComponent::deserialize(sf::Packet &packet)
{
    Component::deserialize(packet);

    packet >> mNetworkID;
}

bool HeroControlComponent::update(float dt)
{
    if (NetworkManager::get()->getType() == NetworkType::CLIENT && mNetworkID == NetworkManager::get()->getNetworkID())
    {
        for (int k = 0; k < sf::Keyboard::KeyCount; k++)
        {
            int state = InputManager::get()->getKeyState(k);
            if (state == ButtonState::PRESSED || state == ButtonState::RELEASED)
            {
                mKeyStates[k] = state;

                // Tell the server about our input
                sf::Packet packet;
                packet << KEYBOARD << k << state;
                NetworkManager::get()->sendToComponent(packet, mGameObject, this);
            }
        }
    }

    processInput();

    return true;
}

void HeroControlComponent::processInput()
{
    if (mKeyStates[sf::Keyboard::W])
    {
        std::cout << "W";
    }

    // Finish the key states
    for (int k = 0; k < sf::Keyboard::KeyCount; k++)
    {
        if (mKeyStates[k] == ButtonState::PRESSED)
            mKeyStates[k] = ButtonState::DOWN;
        else if (mKeyStates[k] == ButtonState::RELEASED)
            mKeyStates[k] = ButtonState::UP;
    }
}

void HeroControlComponent::handlePacket(sf::Packet &packet)
{
    if (NetworkManager::get()->getType() != NetworkType::SERVER) // This absolutely has to be a server
        return;

    // Extract the packet ID
    int ID;
    packet >> ID;

    switch (ID)
    {
        case KEYBOARD:
        {
            int key, state;
            packet >> key >> state;

            mKeyStates[key] = state;

            break;
        }

        case MOUSE:
        {
            break;
        }

        default:
            break;
    }
}

void HeroControlComponent::onPreSolve(GameObject *object, b2Contact* contact, const b2Manifold* oldManifold)
{
}

void HeroControlComponent::onContactBegin(GameObject *object)
{
}

void HeroControlComponent::onContactEnd(GameObject *object)
{
}

Component *HeroControlComponent::createComponent(GameObject *object)
{
    std::cout << "Woah\n";
    return new HeroControlComponent(object, "control", -1);
}
