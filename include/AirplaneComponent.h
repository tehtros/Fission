#ifndef AIRPLANECOMPONENT_H
#define AIRPLANECOMPONENT_H

#include <Core/Component.h>

class AirplaneComponent : public Component
{
    public:
        AirplaneComponent(GameObject *object, std::string name);
        virtual ~AirplaneComponent();

        virtual bool update(float dt);

        virtual void onPreSolve(GameObject *object, b2Contact* contact, const b2Manifold* oldManifold);
        virtual void onContactBegin(GameObject *object);

    protected:
    private:
};

#endif // AIRPLANECOMPONENT_H
