#ifndef EMITTER_H
#define EMITTER_H

#include <SFML/System/Vector2.hpp>
#include <SFML/Graphics/Color.hpp>

namespace rain
{
    class Emitter
    {
        public:
            Emitter();
            virtual ~Emitter();

        protected:
            /// Life time distribution
            int mMinLife, mMaxLife;

            /// Position distribution
            sf::Vector2f mMinPosition, mMaxPosition;

            /// Velocity distribution
            sf::Vector2f mMinVelocity, mMaxVelocity;

            /// Rotation distribution
            float mMinRotation, mMaxRotation;

            /// Rotation speed distribution
            float mMinRotationSpeed, mMaxRotationSpeed;

            /// Color distribution
            sf::Color mMinColor, mMaxColor;

        private:
    };
}

#endif // EMITTER_H
