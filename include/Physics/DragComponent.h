#ifndef DRAGCOMPONENT_H
#define DRAGCOMPONENT_H

#include <Box2D/Dynamics/Joints/b2MouseJoint.h>

#include <Core/Component.h>

class DragComponent : public Component
{
    public:
        DragComponent(GameObject *object, std::string name);
        virtual ~DragComponent();

        virtual bool update(float dt);

    protected:
        b2MouseJoint *mMouseJoint;
        b2Body *mBody;

    private:
};

#endif // DRAGCOMPONENT_H
