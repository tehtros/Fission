/*
GameObject.h
Theodore DeRego
Copyright 2012

Defines a physical object in the game world. Can be derived from for custom functionality.
Also, components can be attached for functionality.
*/


#ifndef GAMEOBJECT_H
#define GAMEOBJECT_H

#include <vector>

#include <SFML/System/Vector2.hpp>
#include <SFML/Network/Packet.hpp>

#include "Core/RefCounted.h"
#include "Core/Component.h"

class GameObject : public RefCounted
{
    public:
        GameObject();
        virtual ~GameObject();

        void serialize(sf::Packet &packet);
        void deserialize(sf::Packet &packet);

        virtual bool update(float dt);
        virtual void onRender(sf::RenderTarget *target, sf::RenderStates states = sf::RenderStates::Default);

        /// Called before a collision is processed
        virtual void onPreSolve(GameObject *object, b2Contact* contact, const b2Manifold* oldManifold);

        /// Called when this collides with another GameObject
        virtual void onContactBegin(GameObject *object);

        /// Called when this object stops colliding with another GameObject
        virtual void onContactEnd(GameObject *object);

        Component *addComponent(Component *component);
        void removeComponent(Component *component);

        /// Getting components by type
        template <typename T> T *getComponent(std::string name = "")
        {
            for (unsigned int c = 0; c < mComponents.size(); c++)
            {
                if (name == "" || mComponents[c]->getName() == name) //no name specified or matching name
                {
                    if (dynamic_cast <T*> (mComponents[c])) //exact match or inherited from
                        return (T*)mComponents[c];
                }
            }
            return 0;
        }

        void kill(){mAlive=false;}

        // Accessors
        int getID(){return mID;}
        bool getAlive(){return mAlive;}
        int getRenderLayer(){return mRenderLayer;}
        bool getSyncNetwork(){return mSyncNetwork;}
        sf::Vector2f getPosition(){return mPosition;}
        float getRotation(){return mRotation;}

        // Mutators
        void setID(int ID){mID=ID;}
        void setRenderLayer(int layer){mRenderLayer=layer;}
        void setSyncNetwork(bool sync){mSyncNetwork=sync;}
        void setPosition(sf::Vector2f position, Component *caller = NULL);
        void setRotation(float rotation, Component *caller = NULL);

    protected:
        /// Generic type for users to give this object
        int mType;

        /// Object's ID - primarily used in networking
        int mID;

        /// Whether or not the object should live to see another frame
        bool mAlive;

        /// This GameObject's render layer
        int mRenderLayer;

        /// Whether or not to sync over the network
        bool mSyncNetwork;

        /// Object's world position
        sf::Vector2f mPosition;

        /// Object rotation
        float mRotation;

        /// The array of components attached to this object
        std::vector <Component*> mComponents;

        /// This is the render target for all components
        sf::Texture *mRenderTarget;

    private:
};

#endif // GAMEOBJECT_H
