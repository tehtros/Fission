#ifndef PLAYERCONTROLCOMPONENT_H
#define PLAYERCONTROLCOMPONENT_H

#include <Core/Component.h>

class PlayerControlComponent : public Component
{
    public:
        PlayerControlComponent(GameObject *object, std::string name);
        virtual ~PlayerControlComponent();

        virtual bool update(float dt);
        //virtual void onRender(sf::RenderTarget *target, sf::RenderStates states);

        //virtual void onPreSolve(GameObject *object, b2Contact* contact, const b2Manifold* oldManifold);
        virtual void onContactBegin(GameObject *object);
        virtual void onContactEnd(GameObject *object);

        void addFollower(){mFollowers++;}

        // Accessors
        int getFollowers(){return mFollowers;}

    protected:
        int mMoveState;
        int mDirection;
        bool mOnGround;
        int mContactCount;
        sf::Clock mContactClock; //time since last contact

        int mFollowers; // Peewees following me

    private:
};

#endif // PLAYERCONTROLCOMPONENT_H
