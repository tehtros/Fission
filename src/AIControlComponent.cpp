#include "AIControlComponent.h"

#include <iostream>
#include <cstdlib>
#include <Core/Math.h>
#include <Core/ResourceManager.h>
#include <Core/GameObject.h>
#include <Rendering/SpriteComponent.h>
#include <Physics/RigidBodyComponent.h>
#include <Logic/WeaponComponent.h>
#include <Logic/ProjectileComponent.h>

#include "Villain.h"

//move states
enum
{
    STAND,
    MOVE,
    JUMP
};

//directions
enum
{
    LEFT,
    RIGHT
};

AIControlComponent::AIControlComponent(GameObject *object, std::string name, std::string lArmTexture, std::string rArmTexture) : HPComponent(object, name)
{
    mTarget = NULL;
    mMoveState = STAND;
    mAimDirection = LEFT;
    mWalkDirection = LEFT;
    mFire = false;

    mContactCount = 0;
    mOnGround = false;

    if (lArmTexture != "")
        mLArmSprite = new sf::Sprite(*ResourceManager::get()->getTexture(lArmTexture));
    else
        mLArmSprite = NULL;

    if (rArmTexture != "")
        mRArmSprite = new sf::Sprite(*ResourceManager::get()->getTexture(rArmTexture));
    else
        mRArmSprite = NULL;

    mArmRotation = 180;

    mMinDist = 2+((float)(rand()%30)/10.f);
    mMaxDist = 8+((float)(rand()%60)/10.f)-3.f;
    mSpeed = 2+((float)(rand()%30)/10.f);
}

AIControlComponent::~AIControlComponent()
{
    //dtor
}

bool AIControlComponent::update(float dt)
{
    SpriteComponent *sprite = mGameObject->getComponent<SpriteComponent>();
    RigidBodyComponent *body = mGameObject->getComponent<RigidBodyComponent>();

    float yVel = body->getBody()->GetLinearVelocity().y;

    if (mContactClock.getElapsedTime().asMilliseconds() < 300)
        mOnGround = true;
    else if (mContactCount == 0)
        mOnGround = false;

    if (mTarget)
    {
        if (fabs(mTarget->getPosition().x-mGameObject->getPosition().x) < mMinDist && mOnGround)
        {
            if (mGameObject->getPosition().x < mTarget->getPosition().x)
            {
                if (mAimDirection == LEFT)
                    mArmRotation = 180-mArmRotation;

                mMoveState = MOVE;
                mAimDirection = RIGHT;
                mWalkDirection = LEFT;
            }
            else if (mGameObject->getPosition().x > mTarget->getPosition().x)
            {
                if (mAimDirection == RIGHT)
                    mArmRotation = 180-mArmRotation;

                mMoveState = MOVE;
                mAimDirection = LEFT;
                mWalkDirection = RIGHT;
            }
        }
        else if (fabs(mTarget->getPosition().x-mGameObject->getPosition().x) > mMaxDist && mOnGround)
        {
            if (mGameObject->getPosition().x < mTarget->getPosition().x)
            {
                if (mAimDirection == LEFT)
                    mArmRotation = 180-mArmRotation;

                mMoveState = MOVE;
                mAimDirection = RIGHT;
                mWalkDirection = RIGHT;
            }
            else if (mGameObject->getPosition().x > mTarget->getPosition().x)
            {
                if (mAimDirection == RIGHT)
                    mArmRotation = 180-mArmRotation;

                mMoveState = MOVE;
                mAimDirection = LEFT;
                mWalkDirection = LEFT;
            }
        }
        else
            mMoveState = STAND;

        float targetRot = (mTarget->getPosition()-mGameObject->getPosition()).getAngleTrig();

        if (mLArmSprite && mRArmSprite)
        {
            if (mArmRotation < targetRot)
                mArmRotation += 25.f*dt;
            else if (mArmRotation > targetRot)
                mArmRotation -= 25.f*dt;
        }
        else
        {
            if (mAimDirection == LEFT)
                mArmRotation = 180;
            else if (mAimDirection == RIGHT)
                mArmRotation = 0;
        }

        if (mAimDirection == LEFT)
        {
            mGameObject->getComponent<WeaponComponent>()->setFirePoint(sf::Vector2f(1.5,-0.75));
            mGameObject->getComponent<WeaponComponent>()->fire(mArmRotation);
        }
        else if (mAimDirection == RIGHT)
        {
            mGameObject->getComponent<WeaponComponent>()->setFirePoint(sf::Vector2f(1.5,0.75));
            mGameObject->getComponent<WeaponComponent>()->fire(mArmRotation);
        }

        if (mTarget->getPosition().x < mGameObject->getPosition().x)
            mAimDirection = LEFT;
        if (mTarget->getPosition().x >= mGameObject->getPosition().x)
            mAimDirection = RIGHT;
    }

    switch (mMoveState)
    {
        case MOVE:
        {
            if (mAimDirection == LEFT)
            {
                sprite->setFrameLoop(12,17);
            }
            else if (mAimDirection == RIGHT)
            {
                sprite->setFrameLoop(18,23);
                sprite->setLoopAnim(true);
            }

            if (mWalkDirection == LEFT)
                body->getBody()->SetLinearVelocity(b2Vec2(-mSpeed,yVel));
            else if (mWalkDirection == RIGHT)
                body->getBody()->SetLinearVelocity(b2Vec2(mSpeed,yVel));

            break;
        }
        case STAND:
        {
            if (mAimDirection == LEFT)
                sprite->setFrameLoop(0,5);
            else if (mAimDirection == RIGHT)
                sprite->setFrameLoop(6,11);
            if (mOnGround)
                body->getBody()->SetLinearVelocity(b2Vec2(0,yVel));
            break;
        }
    }

    return true;
}

void AIControlComponent::onRender(sf::RenderTarget *target, sf::RenderStates states)
{
    //calculate pixel position from GameObject position
    sf::Vector2f objScreenPos = worldToScreen(mGameObject->getPosition());
    sf::Vector2f newPos = sf::Vector2f(objScreenPos.x, objScreenPos.y);
    //newPos.y *= -1;

    if (mAimDirection == LEFT && mLArmSprite)
    {
        mLArmSprite->setPosition(newPos-sf::Vector2f(0,16));
        mLArmSprite->setOrigin(sf::Vector2f(mRArmSprite->getTextureRect().width/2, mRArmSprite->getTextureRect().height/2));
        mLArmSprite->setRotation(180-mArmRotation);
        target->draw(*mLArmSprite, states); //rendahhh!!!!
    }
    else if (mAimDirection == RIGHT && mRArmSprite)
    {
        mRArmSprite->setPosition(newPos-sf::Vector2f(0,16));
        mRArmSprite->setOrigin(sf::Vector2f(mRArmSprite->getTextureRect().width/2, mRArmSprite->getTextureRect().height/2));
        mRArmSprite->setRotation(-mArmRotation);
        target->draw(*mRArmSprite, states); //rendahhh!!!!
    }
}

void AIControlComponent::onPreSolve(GameObject *object, b2Contact* contact, const b2Manifold* oldManifold)
{
    if (object->getType() == BOUND || object->getType() == CHARACTER || object->getType() == ITEM)
        contact->SetEnabled(false);

    // Don't collide with structures that we are jumping up to or are going down from
    if (object->getType() == PLATFORM && mGameObject->getPosition().y <= object->getPosition().y)
    {
        contact->SetEnabled(false);
    }
}

void AIControlComponent::onContactBegin(GameObject *object)
{
    ProjectileComponent *proj = object->getComponent<ProjectileComponent>();
    if (proj) //it's a projectile
    {
        damage(proj->getDamage());
    }

    if (object->getComponent<RigidBodyComponent>())
    {
        mContactClock.restart(); //restart the contact clock
        mContactCount++;
    }
}

void AIControlComponent::onContactEnd(GameObject *object)
{
    if (object->getComponent<RigidBodyComponent>())
        mContactCount--;
}
