#ifndef GAMESTATE_H
#define GAMESTATE_H

#include <Fission.h>

#include "PlayerDatabase.h"
#include "PlanetGenerator.h"

namespace PacketType
{
    enum
    {
        LOGIN = USER_MESSAGE,
        LOGOUT,
        CREATE_PLAYER,
        PLAYER_INPUT,
        CHAT
    };
}

class GameState : public State
{
    public:
        GameState(Game *game, int netType);
        virtual ~GameState();

        virtual void initialize();

        virtual bool update(float dt);

        virtual void onPreRender(sf::RenderTarget *target, sf::RenderStates states);

        virtual void onPostRender(sf::RenderTarget *target, sf::RenderStates states);

        virtual void onConnect(int ID);
        virtual void onDisconnect(int ID);
        virtual void handlePacket(sf::Packet &packet, int connectorID);

        Game *mGame;
        Chat *mChat;
        PlayerDatabase *mPlayerDatabase;
        PlanetGenerator *mPlanetGenerator;

        /// The network type we want
        int mNetworkType;

        /// This client's hero. Null if this is a server
        GameObject *mHero;
};

#endif // GAMESTATE_H
