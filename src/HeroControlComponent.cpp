#include "HeroControlComponent.h"

#include "GameState.h"

enum
{
    KEYBOARD,
    MOUSE_MOVE,
    MOUSE_BUTTON
};

HeroControlComponent::HeroControlComponent(GameObject *object, std::string name, int networkID) : Component(object, name)
{
    for (int k = 0; k < sf::Keyboard::KeyCount; k++)
    {
        mKeyStates[k] = ButtonState::UP;
    }

    mNetworkID = networkID;

    mSpriteComponent = mGameObject->getComponent<SpriteComponent>();
    mBodyComponent = mGameObject->getComponent<RigidBodyComponent>();

    // Add the arms
    mLeftArm = (SpriteComponent*)mGameObject->addComponent(new SpriteComponent(mGameObject, "left", "Content/Textures/left.png", 1, 1));
    mRightArm = (SpriteComponent*)mGameObject->addComponent(new SpriteComponent(mGameObject, "right", "Content/Textures/right.png", 1, 1));
    mLeftArm->setShouldSerialize(false);
    mRightArm->setShouldSerialize(false);

    mGameObject->setSyncNetwork(true); // Sync over the network

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

        // Get the mouse input
        if (InputManager::get()->getMouseMove().getLength() > 0)
        {
            mMousePosition.x = InputManager::get()->getMousePosition().x;
            mMousePosition.y = InputManager::get()->getMousePosition().y;
            mMousePosition = screenToWorld(mMousePosition-RenderingManager::get()->getCameraScreenOffset());

            // Tell the server about our input
            sf::Packet packet;
            packet << MOUSE_MOVE << (float)mMousePosition.x << (float)mMousePosition.y;
            NetworkManager::get()->sendToComponent(packet, mGameObject, this, 0, mNetworkID);
        }

        // Update the camera
        RenderingManager::get()->setCameraPosition(mGameObject->getPosition());
    }

    processInput();

    // Move the arms for pretty arm visuals
    mLeftArm->setPosition(sf::Vector2f(-1.f, 0));
    mRightArm->setPosition(sf::Vector2f(1.f, 0));

    return true;
}

void HeroControlComponent::processInput()
{
    if (mKeyStates[sf::Keyboard::W])
    {
        sf::Vector2f dir = (mMousePosition-mGameObject->getPosition()).normalize();

        mBodyComponent->getBody()->SetLinearVelocity(b2Vec2(dir.x*10.f, dir.y*10.f));
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

        case MOUSE_MOVE:
        {
            packet >> mMousePosition.x >> mMousePosition.y;

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
    return new HeroControlComponent(object, "control", -1);
}
