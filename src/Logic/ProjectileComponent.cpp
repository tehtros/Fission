#include "Logic/ProjectileComponent.h"

#include <GL/gl.h>
#include <Core/ResourceManager.h>
#include <Core/GameObject.h>
#include <Rendering/RenderingManager.h>

#include <iostream>

ProjectileComponent::ProjectileComponent(GameObject *object, std::string name, int dmg, float range) : Component(object, name)
{
    mTeam = -1;

    mDamage = dmg;
    mDistanceLeft = range;
}

ProjectileComponent::~ProjectileComponent()
{
}

bool ProjectileComponent::update(float dt)
{
    sf::Vector2f vel = sf::Vector2f(cos(degToRad(mGameObject->getRotation())), sin(degToRad(mGameObject->getRotation())))*100.f;
    vel *= dt;
    mGameObject->setPosition(mGameObject->getPosition()+vel);

    mDistanceLeft -= sqrt(pow(vel.x,2)+pow(vel.y,2));

    // This bullet has reached it's limit
    if (mDistanceLeft <= 0)
        mGameObject->kill(); // DIIIEEEEE!!!!

    return true;
}
