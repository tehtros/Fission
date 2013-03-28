#ifndef PLAYERDATABASE_H
#define PLAYERDATABASE_H

#include <string>
#include <vector>
#include <Fission.h>

class GameObject;

struct Player
{
    // Login credentials
    std::string mName;
    std::string mPassword;

    int mNetworkID; // This is greater than 0 if logged in, -1 if logged out
    GameObject *mGameObject; // This will be NULL if the player isn't logged in
    sf::Vector2f mSpawnPosition; // Position to log in at
};

class PlayerDatabase
{
    public:
        PlayerDatabase();
        virtual ~PlayerDatabase();

        bool createPlayer(std::string name, std::string password, int networkID);
        void playerLogin(std::string name, std::string password, int networkID);
        void playerLogout(int networkID);

        void spawnPlayer(Player *player);

        Player *findPlayer(std::string name);
        Player *findPlayer(int networkID);

        void addPlayer(Player *player){mPlayers.push_back(player);}

        int getActivePlayerCount(){return mActivePlayers;}
        int getPlayerCount(){return mPlayers.size();}

    protected:
        std::vector <Player*> mPlayers;

        int mActivePlayers;

    private:
};

#endif // PLAYERDATABASE_H
