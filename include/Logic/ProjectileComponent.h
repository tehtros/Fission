#ifndef PROJECTILECOMPONENT_H
#define PROJECTILECOMPONENT_H

#include <LTBL/Light/EmissiveLight.h>
#include <Core/Component.h>

class ProjectileComponent : public Component
{
    public:
        ProjectileComponent(GameObject *object, std::string name, int dmg, float range);
        virtual ~ProjectileComponent();

        virtual bool update(float dt);
        virtual void onRender(sf::RenderTarget *target, sf::RenderStates states);

        // Accessors
        int getTeam(){return mTeam;}
        int getDamage(){return mDamage;}
        float getRange(){return mDistanceLeft;}

        // Mutators
        void setTeam(int team){mTeam=team;}
        void setDamage(int damage){mDamage=damage;}
        void setRange(float range){mDistanceLeft=range;}

    protected:
        /// The team this projectile fights for
        int mTeam;

        /// Amount of damage to deal on a collision
        int mDamage;

        /// Distance left to travel
        float mDistanceLeft;

    private:
};

#endif // PROJECTILECOMPONENT_H
