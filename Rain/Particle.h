#ifndef PARTICLE_H
#define PARTICLE_H

#include <SFML/System/Vector2.hpp>
#include <SFML/Graphics/Color.hpp>

namespace rain
{
    class Particle
    {
        public:
            Particle(int lifeTime);
            virtual ~Particle();

            /**
               @return Life time left of particle in milliseconds
            */
            int getLifeTimeLeft();

            /// Position relative to the ParticleSystem
            sf::Vector2f mPosition;

            /// Rotation of the particle
            float mRotation;

            /// Scale of the particle
            sf::Vector2f mScale;

            /// Color of the particle
            sf::Color mColor;

        private:
            /// Life time left of the particle
            int mLifeTimeLeft;
    };
}

#endif // PARTICLE_H
