#ifndef HEROCONTROLCOMPONENT_H
#define HEROCONTROLCOMPONENT_H

#include <Core/Component.h>
#include <Core/InputManager.h>
#include <Network/NetworkManager.h>

class HeroControlComponent : public Component
{
    public:
        HeroControlComponent(GameObject *object, std::string name, int networkID);
        virtual ~HeroControlComponent();

        virtual void serialize(sf::Packet &packet);
        virtual void deserialize(sf::Packet &packet);

        virtual bool update(float dt);
        //virtual void onRender(sf::RenderTarget *target, sf::RenderStates states);

        virtual void processInput();

        virtual void handlePacket(sf::Packet &packet);

        virtual void onPreSolve(GameObject *object, b2Contact* contact, const b2Manifold* oldManifold);
        virtual void onContactBegin(GameObject *object);
        virtual void onContactEnd(GameObject *object);

        static Component *createComponent(GameObject *object);

        // Accessors
        int getNetworkID(){return mNetworkID;}

        // Mutators
        void setNetworkID(int ID){mNetworkID=ID;}

    protected:
        /// My player's network ID, whether this is on a server or a client
        int mNetworkID;

        /// Key states for networked input
        int mKeyStates[sf::Keyboard::KeyCount];

    private:
};

#endif // HEROCONTROLCOMPONENT_H
