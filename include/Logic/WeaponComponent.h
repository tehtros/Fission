#ifndef WEAPONCOMPONENT_H
#define WEAPONCOMPONENT_H

#include <Core/Component.h>

namespace WeaponType
{
    enum
    {
        INSTANT_HIT,
        PROJECTILE
    };
}

class WeaponComponent : public Component
{
    public:
        WeaponComponent(GameObject *object, std::string name);
        virtual ~WeaponComponent();

        /// Fire this weapon normally
        void fire();

        /// Option to specify the rotation rather than getting it from the owning GameObject
        virtual void fire(float rotation);

        // Accessors

        // Mutators
        void setDamage(int dmg){mDamage=dmg;}
        void setRange(float range){mRange=range;}
        void setInaccuracy(float inaccuracy){mInaccuracy=inaccuracy;}
        void setFirePoint(sf::Vector2f firePoint){mFirePoint=firePoint;}
        void setVisibleBullets(bool visible){mVisibleBullets=visible;}

    protected:
        /// Type of gun: Instant hit, projectile, or melee
        int mType;

        /// How much damage this gun does
        int mDamage;

        /// Range of the bullets
        float mRange;

        /// Spread of the angle the bullets can spray in
        float mInaccuracy;

        /// Cool down time of weapon in milliseconds
        int mCoolDown;

        /// Where the bullets come out
        sf::Vector2f mFirePoint;

        /// The cool down clock
        sf::Clock mCoolDownClock;

        /// Visible bullets
        bool mVisibleBullets;

    private:
};

#endif // WEAPONCOMPONENT_H
