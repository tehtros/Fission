#include <Network/NetworkManager.h>

#include <iostream>
#include <Core/StateManager.h>
#include <Core/GameObject.h>
#include <Core/Component.h>
#include <Scene/SceneManager.h>

NetworkManager *NetworkManager::Instance;

NetworkManager::NetworkManager()
{
    Instance = this;

    mNextID = 1;
    mNetworkID = -1; // Set to -1 for no connection

    enet_initialize();
}

NetworkManager::~NetworkManager()
{
    enet_deinitialize();
}

void NetworkManager::hostServer(int port)
{
    mType = NetworkType::SERVER;

    mServerAddress.host = ENET_HOST_ANY;
    mServerAddress.port = port;

    mHost = enet_host_create(&mServerAddress, 32, 2, 0, 0);

    if (!mHost) // All servers have to do is bind the port
    {
        std::cout << "Error starting server.\n";
        mConnected = false;
    }
    else
    {
        std::cout << "Successfully started server.\n";
        mNetworkID = 0; // Server gets a network ID of 0
        mConnected = true;
    }
}

void NetworkManager::connectClient(std::string ipAddress, int port)
{
    enet_address_set_host(&mServerAddress, ipAddress.c_str());
    mServerAddress.port = port;

    // Create the enet host
    mHost = enet_host_create(NULL, 1, 2, 57600 / 8, 14400 / 8);
    mPeer = enet_host_connect(mHost, &mServerAddress, 2, 0);

    // Send the connection request packet
    if (!mHost || !mPeer)
    {
        std::cout << "Failed to connect to " << ipAddress << std::endl;
        mConnected = false;
        return;
    }

    // Wait up to 5 seconds for the connection attempt to succeed.
    ENetEvent event;
    if (enet_host_service(mHost, &event, 5000) > 0 &&
        event.type == ENET_EVENT_TYPE_CONNECT)
    {
        // Successfully opened connection
    }
    else
    {
        enet_peer_reset(mPeer);
        std::cout << "Connection to " << ipAddress << " failed.\n";
        return;
    }

    // Wait five seconds for network ID
    if (enet_host_service(mHost, &event, 10000) > 0 &&
        event.type == ENET_EVENT_TYPE_RECEIVE)
    {
        sf::Packet packet;
        packet.append(event.packet->data+0, event.packet->dataLength-0);
        packet >> mNetworkID;
        packet.clear();
        std::cout << "Connection to " << ipAddress << " succeeded.\n";
        std::cout << "My ID: " << mNetworkID << std::endl;
    }
    else
    {
        enet_peer_reset(mPeer);
        std::cout << "Connection to " << ipAddress << " failed.\n";
        return;
    }

    // We are now connected
    mConnected = true;
    mType = NetworkType::CLIENT;
}

bool NetworkManager::update(float dt)
{
    if (!mConnected)
        return true;

    ENetEvent event;
    char ipcstr[100];
    std::string IP;

    // Check for events, but don't wait
    for (int netIteration = 0; netIteration < 10000; netIteration++)
    {
        if (!(enet_host_service(mHost, &event, 1) > 0))
            break;

        enet_address_get_host_ip(&event.peer->address, ipcstr, 100);
        IP = ipcstr;

        switch (event.type)
        {
            case ENET_EVENT_TYPE_CONNECT:
            {
                std::cout << "New connector " << mNextID << " from " << IP << ":" << event.peer->address.port << std::endl;

                // Add the new connector
                Connector connector;
                connector.mID = mNextID;
                connector.mIPAddress = IP;
                connector.mPeer = event.peer;
                mConnectors.push_back(connector);
                mNextID++;

                event.peer->data = &mConnectors.back();

                // Send the client its ID
                sf::Packet idPacket;
                idPacket << connector.mID;
                send(idPacket, connector.mID);
                enet_host_flush(mHost);
                idPacket.clear();

                StateManager::get()->getCurrentState()->onConnect(connector.mID);

                break;
            }

            case ENET_EVENT_TYPE_RECEIVE:
            {
                sf::Packet packet;
                packet.append(event.packet->data+0, event.packet->dataLength-0);

                // Extract the packet ID without moving forward in the packet
                int packetID;
                packet >> packetID; // Get packet ID

                switch (packetID)
                {
                    case PacketType::COMPONENT_MESSAGE:
                    {
                        int objID; // GameObject's ID
                        std::string name; // Component name
                        packet >> objID >> name; // Get the essentials
                        GameObject *object = SceneManager::get()->findGameObject(objID);
                        if (object)
                        {
                            Component *component = object->getComponent<Component>(name);
                            if (component)
                                component->handlePacket(packet);
                        }
                        break;
                    }

                    default:
                    {
                        packet.reset();
                        if (mType == NetworkType::SERVER)
                            StateManager::get()->getCurrentState()->handlePacket(packet, ((Connector*)event.peer->data)->mID);
                        else if (mType == NetworkType::CLIENT)
                            StateManager::get()->getCurrentState()->handlePacket(packet, 0);
                        break;
                    }
                }

                enet_packet_destroy(event.packet);
                packet.clear();

                break;
            }

            case ENET_EVENT_TYPE_DISCONNECT:
            {
                if (mType == NetworkType::CLIENT)
                {
                    std::cout << "Disconnected from server\n";
                    mConnected = false;
                }
                else if (mType == NetworkType::SERVER)
                {
                    std::cout << "Connector " << ((Connector*)event.peer->data)->mID << " has disconnected.\n";
                    StateManager::get()->getCurrentState()->onDisconnect(((Connector*)event.peer->data)->mID);
                    removeConnector(((Connector*)event.peer->data)->mID);
                    event.peer->data = NULL;
                }

                break;
            }

            default:
            {
                break;
            }
        }
    }

    return true;
}

void NetworkManager::send(sf::Packet packet, int connectorID, int excludeID, bool reliable)
{
    sf::Packet finalPacket;
    //finalPacket << sf::Int8(0);
    finalPacket.append(packet.getData(), packet.getDataSize());

    // Create the enet packet
    unsigned int flags = ENET_PACKET_FLAG_NO_ALLOCATE;
    if (reliable)
        flags |= ENET_PACKET_FLAG_RELIABLE;

    ENetPacket *enetPacket = enet_packet_create(finalPacket.getData(), finalPacket.getDataSize(), flags);

    if (mType == NetworkType::CLIENT) // Clients send data to server only
    {
        enet_host_broadcast(mHost, 0, enetPacket);
    }
    else if (connectorID > 0) // It's a server and the client is specified. Tell only that client!
    {
        enet_peer_send(findConnector(connectorID).mPeer, 0, enetPacket);
    }
    else // It's a server and the client is unspecified. Broadcast to everyone
    {
        if (excludeID > 0)
        {
            for (unsigned int i = 0; i < mConnectors.size(); i++)
            {
                if (mConnectors[i].mID != excludeID)
                    enet_peer_send(findConnector(connectorID).mPeer, 0, enetPacket);
            }
        }
        else
            enet_host_broadcast(mHost, 0, enetPacket);
    }

    enet_host_flush(mHost);
    finalPacket.clear();
}

void NetworkManager::sendToComponent(sf::Packet packet, GameObject *object, Component *component, int connectorID, int excludeID, bool reliable)
{
    sf::Packet finalPacket;
    finalPacket << PacketType::COMPONENT_MESSAGE;
    finalPacket << object->getID();
    finalPacket << component->getName();
    finalPacket.append(packet.getData(), packet.getDataSize());

    send(finalPacket, connectorID, excludeID, reliable);
}

int NetworkManager::findConnectorID(std::string IP)
{
    for (unsigned int i = 0; i < mConnectors.size(); i++)
    {
        if (mConnectors[i].mIPAddress == IP)
            return mConnectors[i].mID;
    }

    return -1;
}

Connector NetworkManager::findConnector(int ID)
{
    for (unsigned int i = 0; i < mConnectors.size(); i++)
    {
        if (mConnectors[i].mID == ID)
            return mConnectors[i];
    }

    return Connector(); // Return the default invalid connector
}

void NetworkManager::removeConnector(int ID)
{
    for (unsigned int i = 0; i < mConnectors.size(); i++)
    {
        if (mConnectors[i].mID == ID)
        {
            mConnectors.erase(mConnectors.begin()+i);
            return;
        }
    }
}
