#ifndef PLAYERCONTROLCOMPONENT_H
#define PLAYERCONTROLCOMPONENT_H

#include "HPComponent.h"

class PlayerControlComponent : public HPComponent
{
    public:
        PlayerControlComponent(GameObject *object, std::string name);
        virtual ~PlayerControlComponent();

        virtual bool update(float dt);
        virtual void onRender(sf::RenderTarget *target, sf::RenderStates states);

        virtual void onPreSolve(GameObject *object, b2Contact* contact, const b2Manifold* oldManifold);
        virtual void onContactBegin(GameObject *object);
        virtual void onContactEnd(GameObject *object);

    protected:
        int mMoveState;
        int mDirection;
        bool mOnGround;
        int mContactCount;
        sf::Clock mContactClock; //time since last contact
        sf::Clock mGhostClock; //the timer for going through walls
        GameObject *mGround; //ground we are currently on

        GameObject *mLightning;

    private:
};

#endif // PLAYERCONTROLCOMPONENT_H
