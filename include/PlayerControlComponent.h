#ifndef PLAYERCONTROLCOMPONENT_H
#define PLAYERCONTROLCOMPONENT_H

#include <Core/Component.h>

class PlayerControlComponent : public Component
{
    public:
        PlayerControlComponent(GameObject *object, std::string name);
        virtual ~PlayerControlComponent();

        virtual bool update(float dt);

        virtual void onContactBegin(GameObject *object);
        virtual void onContactEnd(GameObject *object);

    protected:
        int mMoveState;
        int mDirection;
        bool mOnGround;
        int mContactCount;
        sf::Clock mContactClock; //time since last contact

    private:
};

#endif // PLAYERCONTROLCOMPONENT_H
