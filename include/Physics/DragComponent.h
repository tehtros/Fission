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

        static Component *createComponent(GameObject *object);

        // Accessors
        b2MouseJoint *getMouseJoint(){return mMouseJoint;}

        // Mutators
        void setMouseJoint(b2MouseJoint *joint){mMouseJoint=joint;}

    protected:
        b2Body *mBody;

        b2MouseJoint *mMouseJoint;

    private:
};

#endif // DRAGCOMPONENT_H
