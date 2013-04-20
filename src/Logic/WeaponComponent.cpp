#include "Logic/WeaponComponent.h"

#include <Core/Math.h>
#include <Core/GameObject.h>
#include <Scene/SceneManager.h>
#include <Physics/PhysicsManager.h>
#include <Logic/ProjectileComponent.h>
#include <Rendering/SpriteComponent.h>

//subclass b2QueryCallback
class MeleeQueryCallback : public b2QueryCallback
{
    public:
        std::vector<b2Body*> foundBodies;

        bool ReportFixture(b2Fixture* fixture)
        {
            foundBodies.push_back( fixture->GetBody() );
            return true; // Keep going to find all fixtures in the query area
        }
};

WeaponComponent::WeaponComponent(GameObject *object, std::string name) : Component(object, name)
{
    mType = WeaponType::INSTANT_HIT;
    mDamage = 1;
    mRange = 100.f;
    mInaccuracy = 5;
    mCoolDown = 100;
    mFirePoint = sf::Vector2f(0.5f,0);

    mTypeName = "WeaponComponent";
}

WeaponComponent::~WeaponComponent()
{
}

void WeaponComponent::fire()
{
    fire(mGameObject->getRotation());
}

void WeaponComponent::fire(float rotation)
{
    if (mCoolDownClock.getElapsedTime().asMilliseconds() < mCoolDown) //not cooled down yet, don't fire
        return;

    mCoolDownClock.restart();

    rotation += (((float)(rand()%100)/100.f)*(mInaccuracy/2))-(mInaccuracy/2);

    sf::Vector2f direction;
    direction.x = cos(degToRad(rotation));
    direction.y = sin(degToRad(rotation));

    sf::Vector2f firePoint = mFirePoint;
    firePoint.rotateBy(rotation, mRotatePoint); //orient the fire point to the rotation of the object

    //create the start end finish points of the bullet
    sf::Vector2f start = mGameObject->getPosition() + firePoint;
    sf::Vector2f end = start + (direction*mRange);

    switch (mType)
    {
        case WeaponType::INSTANT_HIT:
        {
            //set up input
            b2RayCastInput input;
            input.p1 = b2Vec2(start.x, start.y);
            input.p2 = b2Vec2(end.x, end.y);
            input.maxFraction = 1;

            //check every fixture of every body to find closest
            GameObject *hitObject = NULL; //the object we hit
            float closestFraction = 1; //start with end of line as p2
            sf::Vector2f normal; //the normal vector of this intersection
            for (b2Body* b = getGame()->getPhysicsManager()->getWorld()->GetBodyList(); b; b = b->GetNext())
            {
                if (!b->IsActive() || ((GameObject*)b->GetUserData()) == mGameObject) //skip inactive objects and this weapon's object
                    continue;

                for (b2Fixture* f = b->GetFixtureList(); f; f = f->GetNext()) //iterate through each shape in this object
                {
                    b2RayCastOutput output;
                    if (!f->RayCast(&output, input, 0)) //perform the raycast
                        continue;
                    if (output.fraction < closestFraction)
                    {
                        hitObject = ((GameObject*)b->GetUserData());
                        closestFraction = output.fraction;
                        normal = sf::Vector2f(output.normal.x, output.normal.x);
                    }
                }
            }
            sf::Vector2f intersectionPoint = start + (closestFraction * (end - start));

            // Create the projectile
            GameObject *proj = getGame()->getSceneManager()->createGameObject();
            proj->addComponent(new SpriteComponent(proj, "sprite", "Content/Textures/bullet.png"));
            ProjectileComponent *pcomp = new ProjectileComponent(proj, "projectile", mDamage, closestFraction*mRange);
            pcomp->setTeam(mTeam);
            proj->addComponent(pcomp);

            proj->setPosition(start);
            proj->setRotation(rotation);

            // Tell the object that it has taken damage
            if (hitObject)
            {
                // Tell the hit object that he just got hit
                //hitObject->onPreSolve(proj, NULL, NULL);
                hitObject->onContactBegin(proj);
                hitObject->onContactEnd(proj);
            }

            break;
        }
        case WeaponType::PROJECTILE:
        {
            // Create the projectile
            if (mProjectile)
            {
                GameObject *projObj = mProjectile->clone();
                ProjectileComponent *proj = projObj->getComponent<ProjectileComponent>();

                if (proj)
                {
                    proj->setTeam(mTeam);
                    proj->setRange(mRange);
                    proj->setDamage(mDamage);
                }
            }

            break;
        }
    }
}

Component *WeaponComponent::createComponent(GameObject *object)
{
    return new WeaponComponent(object, "weapon");
}
