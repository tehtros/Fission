#include "Physics/PhysicsManager.h"

#include <Core/GameObject.h>

PhysicsManager *PhysicsManager::Instance = NULL;

PhysicsManager::PhysicsManager()
{
    Instance = this;

    mWorld = new b2World(b2Vec2(0, -10)); //gravity = 10
    mWorld->SetContactListener(this);

    mGroundBody = NULL;

    mDragger = NULL;
}

PhysicsManager::~PhysicsManager()
{
    //dtor
}

bool PhysicsManager::update(float dt)
{
    mWorld->Step(dt, 8, 3);

    return true;
}

void PhysicsManager::PreSolve(b2Contact* contact, const b2Manifold* oldManifold)
{
    GameObject *objectA = static_cast <GameObject*> (contact->GetFixtureA()->GetBody()->GetUserData()); //grab the first object
    GameObject *objectB = static_cast <GameObject*> (contact->GetFixtureB()->GetBody()->GetUserData()); //grab the second object

    objectA->onPreSolve(objectB, contact, oldManifold);
    objectB->onPreSolve(objectA, contact, oldManifold);
}

void PhysicsManager::PostSolve(b2Contact* contact, const b2ContactImpulse* impulse)
{
}

void PhysicsManager::BeginContact(b2Contact* contact)
{
    GameObject *objectA = static_cast <GameObject*> (contact->GetFixtureA()->GetBody()->GetUserData()); //grab the first object
    GameObject *objectB = static_cast <GameObject*> (contact->GetFixtureB()->GetBody()->GetUserData()); //grab the second object

    objectA->onContactBegin(objectB);
    objectB->onContactBegin(objectA);
}

void PhysicsManager::EndContact(b2Contact* contact)
{
    GameObject *objectA = static_cast <GameObject*> (contact->GetFixtureA()->GetBody()->GetUserData()); //grab the first object
    GameObject *objectB = static_cast <GameObject*> (contact->GetFixtureB()->GetBody()->GetUserData()); //grab the second object

    objectA->onContactEnd(objectB);
    objectB->onContactEnd(objectA);
}
