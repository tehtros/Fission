#include "EnemyComponent.h"

#include <Core/ResourceManager.h>
#include <Core/GameObject.h>
#include <Rendering/SpriteComponent.h>
#include <Physics/RigidBodyComponent.h>

#include "GameState.h"
#include "PlayerControlComponent.h"

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

EnemyComponent::EnemyComponent(GameObject *object, std::string name, GameState *state) : Component(object, name)
{
    mGameState = state;

    mOnGround = false; // Floating by default
    mContactCount = 0;

    mGameObject->getComponent<SpriteComponent>()->setFrameLoop(0,0);
    mMoveState = STAND;
    mDirection = LEFT;

    mLeader = NULL;
}

EnemyComponent::~EnemyComponent()
{
    //dtor
}

bool EnemyComponent::update(float dt)
{
    // First of all, check to see if I'm dead
    if (mGameObject->getPosition().y < -5)
    {
        mGameObject->kill();
        return true;
    }

    SpriteComponent *sprite = mGameObject->getComponent<SpriteComponent>();
    RigidBodyComponent *body = mGameObject->getComponent<RigidBodyComponent>();

    float yVel = body->getBody()->GetLinearVelocity().y;

    if (mContactClock.getElapsedTime().asMilliseconds() < 300)
        mOnGround = true;
    else if (mContactCount == 0)
        mOnGround = false;

    bool left, right, jump;
    left = right = jump = false;

    if (mLeader)
    {
        float xdist = mLeader->getPosition().x-mGameObject->getPosition().x; // Distance from my leader

        if (xdist < -mFollowDistance)
            left = true;
        else if (xdist > mFollowDistance)
            right = true;

        for (unsigned int j = 0; j < mGameState->getJumpPoints().size(); j++)
        {
            if ((mGameObject->getPosition()-mGameState->getJumpPoints()[j]).getLength() < 0.4f)
            {
                if (mContactClock.getElapsedTime().asMilliseconds() > 200 && (left || right))
                    jump = true;
                else
                {
                    left = false;
                    right = false;
                    jump = false;
                }
            }
        }
    }

    if (left)
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
    else if (right)
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

    if (jump && mOnGround) //jump!
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

void EnemyComponent::onContactBegin(GameObject *object)
{
    if (object->getComponent<RigidBodyComponent>())
    {
        mContactClock.restart(); //restart the contact clock
        mContactCount++;

        if (object->getComponent<PlayerControlComponent>())
        {
            if (object->getPosition().y-0.5 > mGameObject->getPosition().y &&
                abs(object->getPosition().x-mGameObject->getPosition().x) < 0.1)
            {
                // Contact stuff
                object->getComponent<RigidBodyComponent>()->getBody()->SetLinearVelocity(b2Vec2(0,7));
                object->onContactEnd(mGameObject);

                // Following stuff
                mLeader = object;
                mFollowDistance = 4.f+(mLeader->getComponent<PlayerControlComponent>()->getFollowers()*1.f);
                mLeader->getComponent<PlayerControlComponent>()->addFollower();

                // Turn me into a loving peewee - they don't collide with the leader and look prettier
                mGameObject->getComponent<RigidBodyComponent>()->setCollisionGroup(1);
                mGameObject->onContactEnd(object);
                mGameObject->getComponent<SpriteComponent>()->setTexture(ResourceManager::get()->getTexture("Content/Textures/pinkpeewee.png"));
            }
        }
    }
}

void EnemyComponent::onContactEnd(GameObject *object)
{
    if (object->getComponent<RigidBodyComponent>())
        mContactCount--;
}
