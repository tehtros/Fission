#ifndef NETWORKMANAGER_H
#define NETWORKMANAGER_H

#include <enet/enet.h>
#include <SFML/Network.hpp>

#include <Core/Manager.h>

namespace NetworkType
{
    enum
    {
        SERVER,
        CLIENT
    };
};

namespace PacketType
{
    enum
    {
        COMPONENT_MESSAGE,
        USER_MESSAGE
    };
};

struct Connector
{
    int mID;
    std::string mIPAddress;
    ENetPeer *mPeer;
};

class GameObject;
class Component;

class NetworkManager : public Manager
{
    public:
        NetworkManager();
        virtual ~NetworkManager();

        void hostServer(int port);
        void connectClient(std::string ipAddress, int port);

        virtual bool update(float dt);

        void send(sf::Packet packet, int connectorID = 0, int excludeID = 0, bool reliable = true); // connectorID is only relevant to server. It is 0 to send to all clients
        void sendToComponent(sf::Packet packet, GameObject *object, Component *component, int connectorID = 0, int excludeID = 0, bool reliable = true);

        int findConnectorID(std::string IP);
        Connector findConnector(int ID);
        void removeConnector(int ID);

        // Accessors
        int getType(){return mType;} /// Returns the network role of this application - server or client
        bool getConnected(){return mConnected;}
        int getNetworkID(){return mNetworkID;}

        /// The get function for this singleton
        static NetworkManager *get(){return Instance;}

    protected:
        /// Server or client?
        int mType;

        /// Am I connected?
        bool mConnected;

        /// My network ID. This is 0 if I'm the server
        int mNetworkID;

        /// My packet header
        int mHeader;

        /// The network host
        ENetHost *mHost;

        /// If it's a client, the peer
        ENetPeer *mPeer;

        /// The server address if I'm a client
        ENetAddress mServerAddress;

        /// The list of clients if I'm a server
        std::vector <Connector> mConnectors;

        /// The ID for the next connector
        int mNextID;

    private:
        static NetworkManager *Instance;
};

#endif // NETWORKMANAGER_H
