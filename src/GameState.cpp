#include "GameState.h"

#include <iostream>

#include <assert.h>
#include <cstdlib>
#include <ctime>
#include <fstream>
#include <string>
#include <LTBL/Constructs/Vec2f.h>

#include <Fission.h>

#include "HeroControlComponent.h"

GameState::GameState(Game *game, int netType)
{
    srand(time(NULL));
    mGame = game;
    mChat = new Chat(PacketType::CHAT, "Wobble");

    mNetworkType = netType;

    if (mNetworkType == NetworkType::SERVER)
        mPlayerDatabase = new PlayerDatabase;
    else
        mPlayerDatabase = NULL;

    mHero = NULL;
}

GameState::~GameState()
{
    //dtor
}

void GameState::initialize()
{
    SceneManager::get()->registerComponentCreationFunction("HeroControlComponent", HeroControlComponent::createComponent);

    GameObject *ground = SceneManager::get()->createGameObject();
    ground->addComponent(new GlowingShapeComponent(ground, "shape", "Content/groundShape.txt"));
    ground->addComponent(new RigidBodyComponent(ground, "body", "Content/groundBody.txt"));
    ground->setPosition(sf::Vector2f(5,-5));
    PhysicsManager::get()->setGroundBody(ground->getComponent<RigidBodyComponent>()->getBody());

    RenderingManager::get()->setCameraPosition(sf::Vector2f(0.f,0.f));

    // Ambient light
    RenderingManager::get()->getLightSystem()->m_ambientColor = sf::Color(255,255,255);

    PhysicsManager::get()->getWorld()->SetGravity(b2Vec2(0.f,-9.f));

    if (mNetworkType == NetworkType::SERVER)
        NetworkManager::get()->hostServer(50000);
    else if (mNetworkType == NetworkType::CLIENT)
        NetworkManager::get()->connectClient("127.0.0.1", 50000);

    mChat->initialize();
}

bool GameState::update(float dt)
{
    mChat->update();

    return State::update(dt);
}

void GameState::onPreRender(sf::RenderTarget *target, sf::RenderStates states)
{
}

void GameState::onPostRender(sf::RenderTarget *target, sf::RenderStates states)
{
    mChat->draw(target, states);
}

void GameState::onConnect(int ID)
{
    GameObject *player;
    player = SceneManager::get()->createGameObject();
    player->addComponent(new SpriteComponent(player, "sprite", "Content/Textures/averysprite.png", 36, 6));
    player->addComponent(new RigidBodyComponent(player, "body", "Content/beingBody.txt"));
    player->addComponent(new DragComponent(player, "drag"));
    player->addComponent(new HeroControlComponent(player, "control", ID));
    player->setPosition(sf::Vector2f(0,5.f));
    player->getComponent<SpriteComponent>()->setAnimDelay(100);
    player->getComponent<RigidBodyComponent>()->getBody()->SetFixedRotation(true);
    player->getComponent<RigidBodyComponent>()->setCollisionGroup(1);

    sf::Packet packet;
    packet << PacketType::CREATE_OBJECT;
    player->serialize(packet);
    NetworkManager::get()->send(packet);
}

void GameState::onDisconnect(int ID)
{
}

void GameState::handlePacket(sf::Packet &packet, int connectorID)
{
    int packetID;
    packet >> packetID;

    if (NetworkManager::get()->getType() == NetworkType::SERVER)
    {
        switch (packetID)
        {
            case PacketType::LOGIN:
            {
                break;
            }

            case PacketType::LOGOUT:
            {
                break;
            }

            case PacketType::PLAYER_INPUT:
            {
                int playerID;
                packet >> playerID;
                Player *player = mPlayerDatabase->findPlayer(playerID);
                if (playerID == connectorID && player) // Extra security
                    (player->mGameObject->getComponent<HeroControlComponent>("control"))->handlePacket(packet);

                break;
            }

            case PacketType::CHAT:
            {
                int playerID;
                std::string msg;
                packet >> playerID;
                packet >> msg;
                if (playerID == connectorID) // Just for extra security
                    mChat->addMessage(playerID, msg);
                break;
            }

            default:
            {
                std::cout << "Invalid packet ID: " << packetID << std::endl;
                break;
            }
        }
    }
    else if (NetworkManager::get()->getType() == NetworkType::CLIENT)
    {
        switch (packetID)
        {
            case PacketType::CREATE_OBJECT:
            {
                GameObject *object = SceneManager::get()->createGameObject();
                object->deserialize(packet);
                break;
            }

            case PacketType::CREATE_PLAYER:
            {
                int objID, playerID;
                packet >> playerID;
                packet >> objID;
                SceneManager::get()->findGameObject(objID);

                break;
            }

            case PacketType::PLAYER_INPUT:
            {
                int playerID;
                packet >> playerID;
                Player *player = mPlayerDatabase->findPlayer(playerID);
                if (player)
                    (player->mGameObject->getComponent<HeroControlComponent>("control"))->handlePacket(packet);

                break;
            }

            case PacketType::CHAT:
            {
                int playerID;
                std::string msg;
                packet >> playerID;
                packet >> msg;
                // Only add messages from other people
                if (NetworkManager::get()->getType() == NetworkType::CLIENT && NetworkManager::get()->getNetworkID() != playerID)
                    mChat->addMessage(playerID, msg);
                break;
            }

            default:
            {
                std::cout << "Invalid packet ID: " << packetID << std::endl;
                break;
            }
        }
    }
}
