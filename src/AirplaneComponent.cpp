#include "AirplaneComponent.h"

#include "Villain.h"

AirplaneComponent::AirplaneComponent(GameObject *object, std::string name) : Component(object, name)
{
    //ctor
}

AirplaneComponent::~AirplaneComponent()
{
    //dtor
}

bool AirplaneComponent::update(float dt)
{
    if (!mGameObject->getComponent<DragComponent>()->getMouseJoint())
    {
        b2Vec2 velocity;
        velocity.x = cos(degToRad(mGameObject->getRotation()))*13.f;
        velocity.y = sin(degToRad(mGameObject->getRotation()))*13.f;
        mGameObject->getComponent<RigidBodyComponent>()->getBody()->SetLinearVelocity(velocity);
    }
    return true;
}

void AirplaneComponent::onPreSolve(GameObject *object, b2Contact* contact, const b2Manifold* oldManifold)
{
    if (object->getType() == PLATFORM || object->getType() == BOUND || object->getType() == ITEM)
        contact->SetEnabled(false);
}

void AirplaneComponent::onContactBegin(GameObject *object)
{
    if (object->getType() != PLATFORM && object->getType() != BOUND && !object->getComponent<ProjectileComponent>())
    {
        b2Vec2 offset;
        offset.x = cos(degToRad(mGameObject->getRotation()))*4.f;
        offset.y = sin(degToRad(mGameObject->getRotation()))*4.f;
        spawnExplosion2(mGameObject->getPosition());
        spawnExplosion2(mGameObject->getPosition()+sf::Vector2f(offset.x, offset.y));
        spawnExplosion2(mGameObject->getPosition()-sf::Vector2f(offset.x, offset.y));
        mGameObject->kill();
    }
}
