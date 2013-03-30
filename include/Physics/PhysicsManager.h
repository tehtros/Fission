#ifndef PHYSICSMANAGER_H
#define PHYSICSMANAGER_H

#include <Box2D/Box2D.h>

#include <Core/Manager.h>

class DragComponent;

class PhysicsManager : public Manager, b2ContactListener
{
    public:
        PhysicsManager();
        virtual ~PhysicsManager();

        virtual bool update(float dt);

        //Physics collision detection stuff
        virtual void PreSolve(b2Contact* contact, const b2Manifold* oldManifold);
        virtual void PostSolve(b2Contact* contact, const b2ContactImpulse* impulse);
        virtual void BeginContact(b2Contact* contact);
        virtual void EndContact(b2Contact* contact);

        void resetTime(){mTime=0;}

        // Accessors
        b2World *getWorld(){return mWorld;}
        int getTime(){return mTime;}
        b2Body *getGroundBody(){return mGroundBody;}
        DragComponent *getDragger(){return mDragger;}

        // Mutators
        void setGroundBody(b2Body *body){mGroundBody=body;}
        void setDragger(DragComponent *dragger){mDragger=dragger;}

        static PhysicsManager *get(){return Instance;}

    protected:
        b2World *mWorld;

        /// Number of frames that have passed since the time was reset
        int mTime;

        b2Body *mGroundBody;

        /// The one and only mouse drag component
        DragComponent *mDragger;

    private:
        static PhysicsManager *Instance;
};

#endif // PHYSICSMANAGER_H
