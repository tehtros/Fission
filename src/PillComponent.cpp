#include "PillComponent.h"

#include "Villain.h"

PillComponent::PillComponent(GameObject *object, std::string name) : Component(object, name)
{
    //ctor
}

PillComponent::~PillComponent()
{
    //dtor
}

bool PillComponent::update(float dt)
{
    if (mLifeClock.getElapsedTime().asMilliseconds() > 10000)
        mGameObject->kill();

    return true;
}

void PillComponent::onContactBegin(GameObject *object)
{
    if (object->getTeam() == 0 && object->getComponent<HPComponent>())
    {
        object->getComponent<HPComponent>()->setHP(object->getComponent<HPComponent>()->getHP()+200);
        mGameObject->kill();
    }
}
