#ifndef VILLAIN_H_INCLUDED
#define VILLAIN_H_INCLUDED

#include <Fission.h>
#include "PlayerControlComponent.h"
#include "AIControlComponent.h"
#include "AirplaneComponent.h"
#include "PillComponent.h"

enum
{
    GROUND,
    BOUND,
    PLATFORM,
    CHARACTER,
    ITEM
};

enum
{
    MARINE,
    MECH,
    TANK
};

inline GameObject* spawnMarine(sf::Vector2f pos)
{
    GameObject *marine = SceneManager::get()->createGameObject();
    marine->addComponent(new SpriteComponent(marine, "sprite", "Content/Textures/marine.png", 24, 6));
    marine->addComponent(new RigidBodyComponent(marine, "body", "Content/marineBody.txt"));
    marine->addComponent(new AIControlComponent(marine, "control", "Content/Textures/arm.png", "Content/Textures/arm2.png"));
    marine->addComponent(new DragComponent(marine, "drag"));
    marine->addComponent(new WeaponComponent(marine, "weapon"));
    marine->setPosition(pos);
    marine->getComponent<SpriteComponent>()->setAnimDelay(100);
    marine->getComponent<RigidBodyComponent>()->getBody()->SetFixedRotation(true);
    marine->getComponent<AIControlComponent>()->setAIType(MARINE);
    marine->getComponent<WeaponComponent>()->setInaccuracy(10.f);
    marine->setType(CHARACTER);
    marine->setTeam(1);

    return marine;
}

inline GameObject* spawnMech(sf::Vector2f pos)
{
    GameObject *mech = SceneManager::get()->createGameObject();
    mech->addComponent(new SpriteComponent(mech, "sprite", "Content/Textures/mech.png", 24, 6));
    mech->addComponent(new RigidBodyComponent(mech, "body", "Content/mechBody.txt"));
    mech->addComponent(new AIControlComponent(mech, "control", "", ""));
    mech->addComponent(new DragComponent(mech, "drag"));
    mech->addComponent(new WeaponComponent(mech, "weapon"));
    mech->setPosition(pos);
    mech->getComponent<SpriteComponent>()->setAnimDelay(100);
    mech->getComponent<RigidBodyComponent>()->getBody()->SetFixedRotation(true);
    mech->getComponent<AIControlComponent>()->setAIType(MECH);
    mech->getComponent<HPComponent>()->setHP(400);
    mech->getComponent<WeaponComponent>()->setInaccuracy(30.f);
    mech->setType(CHARACTER);
    mech->setTeam(1);
    return mech;
}

inline GameObject* spawnExplosion1(sf::Vector2f pos)
{
    GameObject *exp = SceneManager::get()->createGameObject();
    exp->addComponent(new SpriteComponent(exp, "sprite", "Content/Textures/explosion0.png", 32, 8));
    exp->setPosition(pos);
    exp->getComponent<SpriteComponent>()->setAnimDelay(50);
    exp->getComponent<SpriteComponent>()->setLoopAnim(false);
    exp->getComponent<SpriteComponent>()->setKillOnAnimFinish(true);

    for (unsigned int o = 0; o < SceneManager::get()->getCurrentScene()->getGameObjects().size(); o++)
    {
        GameObject *object = SceneManager::get()->getCurrentScene()->getGameObjects()[o];
        float distance = (pos-object->getPosition()).getLength();

        if (object->getComponent<RigidBodyComponent>() && distance < 7.f && object->getTeam() != 0 && object->getType() != ITEM)
        {
            sf::Vector2f vel = -(pos-object->getPosition()).normalize()*8.f;
            vel.y += 3;
            object->getComponent<RigidBodyComponent>()->getBody()->SetLinearVelocity(b2Vec2(vel.x, vel.y));
        }

        if (object->getComponent<HPComponent>() && distance < 2.f && object->getTeam() != 0)
        {
            object->getComponent<HPComponent>()->damage(100);
        }
    }

    return exp;
}

inline GameObject* spawnExplosion2(sf::Vector2f pos)
{
    GameObject *exp = SceneManager::get()->createGameObject();
    exp->addComponent(new SpriteComponent(exp, "sprite", "Content/Textures/explosion1.png", 64, 8));
    exp->setPosition(pos);
    exp->getComponent<SpriteComponent>()->setAnimDelay(30);
    exp->getComponent<SpriteComponent>()->setLoopAnim(false);
    exp->getComponent<SpriteComponent>()->setKillOnAnimFinish(true);

    for (unsigned int o = 0; o < SceneManager::get()->getCurrentScene()->getGameObjects().size(); o++)
    {
        GameObject *object = SceneManager::get()->getCurrentScene()->getGameObjects()[o];
        float distance = (pos-object->getPosition()).getLength();

        if (object->getComponent<RigidBodyComponent>() && distance < 7.f && object->getType() != ITEM)
        {
            sf::Vector2f vel = -(pos-object->getPosition()).normalize()*8.f;
            vel.y += 3;
            object->getComponent<RigidBodyComponent>()->getBody()->SetLinearVelocity(b2Vec2(vel.x, vel.y));
        }

        if (object->getComponent<HPComponent>() && distance < 4.f && object->getTeam() != 0)
        {
            object->getComponent<HPComponent>()->damage(100);
        }
    }

    return exp;
}

inline GameObject* spawnAirPlane(sf::Vector2f pos)
{
    GameObject *plane = SceneManager::get()->createGameObject();
    plane->addComponent(new SpriteComponent(plane, "sprite", "Content/Textures/plane.png", 1, 1));
    plane->addComponent(new RigidBodyComponent(plane, "body", "Content/planeBody.txt"));
    plane->addComponent(new AirplaneComponent(plane, "plane"));
    plane->addComponent(new DragComponent(plane, "drag"));
    plane->setPosition(pos);
    plane->setType(CHARACTER);
    return plane;
}

inline GameObject* spawnPill(sf::Vector2f pos)
{
    GameObject *pill = SceneManager::get()->createGameObject();
    pill->addComponent(new SpriteComponent(pill, "sprite", "Content/Textures/health.png", 1, 1));
    pill->addComponent(new RigidBodyComponent(pill, "body", "Content/pillBody.txt"));
    pill->addComponent(new PillComponent(pill, "pill"));
    pill->setPosition(pos);
    pill->setType(ITEM);
    return pill;
}

#endif // VILLAIN_H_INCLUDED
