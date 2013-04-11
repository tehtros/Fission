#include "Logic/ProjectileComponent.h"

#include <GL/gl.h>
#include <Core/ResourceManager.h>
#include <Core/GameObject.h>
#include <Rendering/RenderingManager.h>

#include <iostream>

ProjectileComponent::ProjectileComponent(GameObject *object, std::string name, int dmg, float range, bool visual) : Component(object, name)
{
    mTeam = -1;

    mDamage = dmg;
    mDistanceLeft = range;
    mVisual = false;

    // Create an emissive light
    if (mVisual)
    {
        mEmissiveLight = new ltbl::EmissiveLight();

        mEmissiveLight->SetTexture(ResourceManager::get()->getTexture("Content/Textures/bullet.png"));

        mEmissiveLight->m_intensity = 1.1f;

        RenderingManager::get()->getLightSystem()->AddEmissiveLight(mEmissiveLight);
    }
    else
        mEmissiveLight = NULL;
}

ProjectileComponent::~ProjectileComponent()
{
    if (mEmissiveLight)
        RenderingManager::get()->getLightSystem()->RemoveEmissiveLight(mEmissiveLight);
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

void ProjectileComponent::onRender(sf::RenderTarget *target, sf::RenderStates states)
{
    // Update the emissive light's position
    if (mEmissiveLight)
    {
        sf::Vector2u screenSize = target->getSize();

        sf::Vector2f newPos = mGameObject->getPosition()*RenderingManager::get()->getPTU();
        newPos.y *= -1;
        newPos += RenderingManager::get()->getCameraScreenOffset();

        Vec2f lightPos = Vec2f(newPos.x, newPos.y);

        mEmissiveLight->SetCenter(Vec2f(lightPos.x, screenSize.y-lightPos.y));
        mEmissiveLight->SetRotation(mGameObject->getRotation());
    }
}
