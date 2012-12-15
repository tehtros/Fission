#include "Physics/DragComponent.h"

#include <Core/Math.h>
#include <Core/InputManager.h>
#include <Core/GameObject.h>
#include <Physics/PhysicsManager.h>
#include <Physics/RigidBodyComponent.h>
#include <iostream>

DragComponent::DragComponent(GameObject *object, std::string name) : Component(object, name)
{
    mMouseJoint = NULL;
    mBody = mGameObject->getComponent<RigidBodyComponent>()->getBody();
}

DragComponent::~DragComponent()
{
    //dtor
}

bool DragComponent::update(float dt)
{
    bool isTouched = false;

    // Real world coordinate from mouse position
    sf::Vector2f mouseWorld = screenToWorld(InputManager::get()->getMousePosition());
    b2Vec2 mousePos = b2Vec2(mouseWorld.x, mouseWorld.y);

    if (InputManager::get()->getLMBState() == ButtonState::PRESSED)
    {
        // Test the point in our body's fixtures
        for (b2Fixture* f = mBody->GetFixtureList(); f != NULL; f = f->GetNext())
        {
            if (f->TestPoint(mousePos))
            {
                isTouched = true;
                break;
            }
        }
    }

    if (isTouched && mMouseJoint == NULL) // Just tapped the object
    {
        b2MouseJointDef mousedef;

        mousedef.bodyA = PhysicsManager::get()->getGroundBody();
        mousedef.bodyB = mBody;

        mousedef.target = mousePos;

        mousedef.collideConnected = true;

        mousedef.maxForce = 300 * mousedef.bodyB->GetMass();

        mBody->SetAwake(true);
        mMouseJoint = (b2MouseJoint*)PhysicsManager::get()->getWorld()->CreateJoint(&mousedef);
    }
    else if (InputManager::get()->getLMBDown() && mMouseJoint != NULL) // We are dragging this component around
    {
        mMouseJoint->SetTarget(mousePos);
        mBody->SetAwake(true);
    }
    else if (InputManager::get()->getLMBState() == ButtonState::RELEASED && mMouseJoint != NULL)
    {
        PhysicsManager::get()->getWorld()->DestroyJoint(mMouseJoint);
        mMouseJoint = NULL;
    }

    return true;
}
