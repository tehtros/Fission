#include "Physics/DragComponent.h"

#include <Core/Math.h>
#include <Core/InputManager.h>
#include <Core/GameObject.h>
#include <Rendering/RenderingManager.h>
#include <Physics/PhysicsManager.h>
#include <Physics/RigidBodyComponent.h>
#include <iostream>

DragComponent::DragComponent(GameObject *object, std::string name) : Component(object, name)
{
    mMouseJoint = NULL;
    mBody = mGameObject->getComponent<RigidBodyComponent>()->getBody();

    mTypeName = "DragComponent";
}

DragComponent::~DragComponent()
{
    PhysicsManager::get()->setDragger(NULL);
}

bool DragComponent::update(float dt)
{
    bool isTouched = false;

    // Real world coordinate from mouse position
    //sf::Vector2f camOffset =
    sf::Vector2f mouseScreen = sf::Vector2f(InputManager::get()->getMousePosition().x, InputManager::get()->getMousePosition().y)-
                                RenderingManager::get()->getCameraScreenOffset();
    sf::Vector2f mouseWorld = screenToWorld(mouseScreen);
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

    if (isTouched && !mMouseJoint) // Just tapped the object
    {
        b2MouseJointDef mousedef;

        mousedef.bodyA = PhysicsManager::get()->getGroundBody();
        mousedef.bodyB = mBody;

        mousedef.target = mousePos;

        mousedef.collideConnected = true;

        mousedef.maxForce = 300 * mousedef.bodyB->GetMass();

        mBody->SetAwake(true);
        mMouseJoint = (b2MouseJoint*)PhysicsManager::get()->getWorld()->CreateJoint(&mousedef);

        PhysicsManager::get()->setDragger(this);
    }
    else if (InputManager::get()->getLMBDown() && mMouseJoint) // We are dragging this component around
    {
        mMouseJoint->SetTarget(mousePos);
        mBody->SetAwake(true);
    }
    else if (InputManager::get()->getLMBState() == ButtonState::RELEASED && mMouseJoint)
    {
        PhysicsManager::get()->getWorld()->DestroyJoint(mMouseJoint);
        PhysicsManager::get()->setDragger(NULL);
        mMouseJoint = NULL;
    }

    return true;
}

Component *DragComponent::createComponent(GameObject *object)
{
    return new DragComponent(object, "drag");
}
