#include "HPComponent.h"

HPComponent::HPComponent(GameObject *object, std::string name) : Component(object, name)
{
    mHP = 100;
}

HPComponent::~HPComponent()
{
    //dtor
}
