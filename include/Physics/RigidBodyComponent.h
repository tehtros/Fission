#ifndef RIGIDBODYCOMPONENT_H
#define RIGIDBODYCOMPONENT_H

#include <Box2D/Box2D.h>

#include "Core/Component.h"

class RigidBodyComponent : public Component
{
    public:
        RigidBodyComponent(GameObject *object, std::string name, std::string bodyFile);
        virtual ~RigidBodyComponent();

        virtual bool update(float dt);

        virtual void onSetPosition(sf::Vector2f position);
        virtual void onSetRotation(float rotation);

        //accessors
        b2Body *getBody(){return mBody;}

    protected:
        b2Body *mBody; //Box2D body
    private:
};

#endif // RIGIDBODYCOMPONENT_H
