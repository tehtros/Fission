#include "PlayerControlComponent.h"

#include <Core/InputManager.h>
#include <Core/GameObject.h>
#include <Rendering/SpriteComponent.h>
#include <Physics/RigidBodyComponent.h>

//move states
enum
{
    STAND,
    MOVE,
    TURN,
    JUMP
};

//directions
enum
{
    LEFT,
    RIGHT
};

PlayerControlComponent::PlayerControlComponent(GameObject *object, std::string name) : Component(object, name)
{
    mOnGround = false; // Floating by default
    mContactCount = 0;

    mGameObject->getComponent<SpriteComponent>()->setFrameLoop(0,0);
    mMoveState = STAND;
    mDirection = LEFT;
}

PlayerControlComponent::~PlayerControlComponent()
{
    //dtor
}

bool PlayerControlComponent::update(float dt)
{
    SpriteComponent *sprite = mGameObject->getComponent<SpriteComponent>();
    RigidBodyComponent *body = mGameObject->getComponent<RigidBodyComponent>();

    float yVel = body->getBody()->GetLinearVelocity().y;

    if (mContactClock.getElapsedTime().asMilliseconds() < 300)
        mOnGround = true;
    else if (mContactCount == 0)
        mOnGround = false;

    if (InputManager::get()->getKeyDown(sf::Keyboard::A))
    {
        if (!mOnGround) //not on the ground
        {
            mDirection = LEFT;
            body->getBody()->SetLinearVelocity(b2Vec2(-5,yVel));
        }
        else
        {
            if (mDirection == LEFT)
            {
                sprite->setFrameLoop(12,17);
                sprite->setLoopAnim(true);
                body->getBody()->SetLinearVelocity(b2Vec2(-5,yVel));
                mMoveState = MOVE;
            }
            else //do turning animation
            {
                mMoveState = TURN;
            }
        }
    }
    else if (InputManager::get()->getKeyDown(sf::Keyboard::D))
    {
        if (!mOnGround)
        {
            mDirection = RIGHT;
            body->getBody()->SetLinearVelocity(b2Vec2(5,yVel));
        }
        else
        {
            if (mDirection == RIGHT)
            {
                sprite->setFrameLoop(18,23);
                sprite->setLoopAnim(true);
                body->getBody()->SetLinearVelocity(b2Vec2(5,yVel));
                mMoveState = MOVE;
            }
            else //do turning animation
            {
                mMoveState = TURN;
            }
        }
    }
    else if (mMoveState != TURN && mOnGround)
    {
        if (mDirection == LEFT)
            sprite->setFrameLoop(0,5);
        else if (mDirection == RIGHT)
            sprite->setFrameLoop(6,11);
        body->getBody()->SetLinearVelocity(b2Vec2(0,yVel));
        mMoveState = STAND;
    }
    else if (!mOnGround)
    {
    }

    if (InputManager::get()->getKeyState(sf::Keyboard::W) == ButtonState::PRESSED && mOnGround) //jump!
    {
        mMoveState = JUMP;
        body->getBody()->SetLinearVelocity(b2Vec2(0,15));
    }

    //temporary states that aren't controlled with the keyboard
    if (mMoveState == TURN)
    {
        if (mDirection == LEFT)
        {
            sprite->setFrameLoop(24,26);
            sprite->setLoopAnim(false);
            if (sprite->getAnimFinished())
            {
                mMoveState = STAND;
                mDirection = RIGHT;
            }
        }
        else if (mDirection == RIGHT)
        {
            sprite->setFrameLoop(27,29);
            sprite->setLoopAnim(false);
            if (sprite->getAnimFinished())
            {
                mMoveState = STAND;
                mDirection = LEFT;
            }
        }
    }
    if (mMoveState == JUMP)
    {
        if (mOnGround)
            mMoveState = STAND;
    }

    if (!mOnGround)
    {
        if (mDirection == LEFT)
        {
            if (body->getBody()->GetLinearVelocity().y >= 0) //going up
            {
                sprite->setFrameLoop(33,33);
                sprite->setLoopAnim(false);
            }
            else if (body->getBody()->GetLinearVelocity().y < 0) //going down
            {
                sprite->setFrameLoop(34,35);
                sprite->setLoopAnim(false);
            }
        }
        else if (mDirection == RIGHT)
        {
            if (body->getBody()->GetLinearVelocity().y >= 0) //going up
            {
                sprite->setFrameLoop(30,30);
                sprite->setLoopAnim(false);
            }
            else if (body->getBody()->GetLinearVelocity().y < 0) //going down
            {
                sprite->setFrameLoop(31,32);
                sprite->setLoopAnim(false);
            }
        }
    }

    return true;
}

void PlayerControlComponent::onContactBegin(GameObject *object)
{
    if (object->getComponent<RigidBodyComponent>())
    {
        mContactClock.restart(); //restart the contact clock
        mContactCount++;
    }
}

void PlayerControlComponent::onContactEnd(GameObject *object)
{
    if (object->getComponent<RigidBodyComponent>())
        mContactCount--;
}
