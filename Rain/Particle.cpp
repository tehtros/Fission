#include "Particle.h"

namespace rain
{
    Particle::Particle(int lifeTime)
    {
        mRotation = 0;
        mScale = sf::Vector2f(1,1);
        mColor = sf::Color::White;
        mLifeTimeLeft = lifeTime;
    }

    Particle::~Particle()
    {
        //dtor
    }

    int Particle::getLifeTimeLeft()
    {
        return mLifeTimeLeft;
    }
}
