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

        static Component *createComponent(GameObject *object);

        // Accessors
        int getType(){return mType;}
        int getTeam(){return mTeam;}

        // Mutators
        void setType(int type){mType=type;}
        void setTeam(int team){mTeam=team;}
        void setDamage(int dmg){mDamage=dmg;}
        void setRange(float range){mRange=range;}
        void setInaccuracy(float inaccuracy){mInaccuracy=inaccuracy;}
        void setFirePoint(sf::Vector2f firePoint){mFirePoint=firePoint;}
        void setRotatePoint(sf::Vector2f rotatePoint){mRotatePoint=rotatePoint;}
        void setProjectile(GameObject *projectile){mProjectile=projectile;}

    protected:
        /// Type of gun: Instant hit, projectile, or melee
        int mType;

        /// The team this weapon fights for!
        int mTeam;

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

        /// Rotate point
        sf::Vector2f mRotatePoint;

        /// The cool down clock
        sf::Clock mCoolDownClock;

        /// The projectile GameObject. Should NOT actively be in the scene
        GameObject *mProjectile;

    private:
};

#endif // WEAPONCOMPONENT_H
