#include "GameState.h"

#include <iostream>

#include <assert.h>
#include <cstdlib>
#include <ctime>
#include <fstream>
#include <string>

#include <Fission.h>

GameState::GameState(Game *game) : State(game)
{
}

GameState::~GameState()
{
}

void GameState::initialize()
{
}

bool GameState::update(float dt)
{
    return State::update(dt);
}

void GameState::onPreRender(sf::RenderTarget *target, sf::RenderStates states)
{
}

void GameState::onPostRender(sf::RenderTarget *target, sf::RenderStates states)
{
}

void GameState::onConnect(int ID)
{
    if (getGame()->getNetworkManager()->getType() == NetworkType::SERVER)
    {
    }
    else if (getGame()->getNetworkManager()->getType() == NetworkType::CLIENT)
    {
    }
}

void GameState::onDisconnect(int ID)
{
    if (getGame()->getNetworkManager()->getType() == NetworkType::SERVER)
    {
    }
    else if (getGame()->getNetworkManager()->getType() == NetworkType::CLIENT)
    {
    }
}

void GameState::handlePacket(sf::Packet &packet, int connectorID)
{
    int packetID;
    packet >> packetID;

    if (getGame()->getNetworkManager()->getType() == NetworkType::SERVER)
    {
        switch (packetID)
        {
            default:
            {
                std::cout << "Invalid packet ID: " << packetID << std::endl;
                break;
            }
        }
    }
    else if (getGame()->getNetworkManager()->getType() == NetworkType::CLIENT)
    {
        switch (packetID)
        {
            default:
            {
                std::cout << "Invalid packet ID: " << packetID << std::endl;
                break;
            }
        }
    }
}
