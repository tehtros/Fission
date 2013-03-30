#ifndef RIGIDBODYCOMPONENT_H
#define RIGIDBODYCOMPONENT_H

#include <Box2D/Box2D.h>

#include "Core/Component.h"

class RigidBodyComponent : public Component
{
    public:
        RigidBodyComponent(GameObject *object, std::string name, std::string bodyFile);
        RigidBodyComponent(GameObject *object, std::string name, b2Body *body);
        virtual ~RigidBodyComponent();

        virtual void serialize(sf::Packet &packet);
        virtual void deserialize(sf::Packet &packet);

        virtual bool update(float dt);

        virtual void onSetPosition(sf::Vector2f position);
        virtual void onSetRotation(float rotation);

        static Component *createComponent(GameObject *object);

        // Accessors
        b2Body *getBody(){return mBody;}
        int getCollisionGroup(){return mCollisionGroup;}

        // Mutators
        void setCollisionGroup(int group){mCollisionGroup=group;}

    protected:
        b2Body *mBody; //Box2D body
        int mCollisionGroup;

    private:
};

#endif // RIGIDBODYCOMPONENT_H
