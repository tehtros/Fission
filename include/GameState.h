#ifndef GAMESTATE_H
#define GAMESTATE_H

#include <Fission.h>

class GameState : public State
{
    public:
        GameState(Game *game);
        virtual ~GameState();

        virtual void initialize();

        virtual bool update(float dt);

        virtual void onPreRender(sf::RenderTarget *target, sf::RenderStates states);

        virtual void onPostRender(sf::RenderTarget *target, sf::RenderStates states);

        virtual void onConnect(int ID);
        virtual void onDisconnect(int ID);
        virtual void handlePacket(sf::Packet &packet, int connectorID);

    protected:
        Game *mGame;
};

#endif // GAMESTATE_H
