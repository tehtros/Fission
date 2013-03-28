#include "PlayerDatabase.h"

#include "HeroControlComponent.h"

PlayerDatabase::PlayerDatabase()
{
    mActivePlayers = 0;
}

PlayerDatabase::~PlayerDatabase()
{
    //dtor
}

bool PlayerDatabase::createPlayer(std::string name, std::string password, int networkID)
{
    // Make sure this player doesn't already exist
    Player *player = findPlayer(name);
    if (player)
        return false;

    player = new Player;
    player->mName = name;
    player->mPassword = password;
    player->mNetworkID = networkID;
    player->mSpawnPosition = sf::Vector2f(0,5);

    // Create GameObject
    spawnPlayer(player);

    mPlayers.push_back(player);

    mActivePlayers++;

    return true;
}

void PlayerDatabase::playerLogin(std::string name, std::string password, int networkID)
{
    Player *player = findPlayer(name);

    if (player)
    {
        if (player->mPassword == password)
        {
            player->mNetworkID = networkID;

            // Create GameObject
            spawnPlayer(player);

            mActivePlayers++;
        }
        else
        {
            // TODO: Invalid password
        }
    }
    else
    {
        // TODO: Invalid username
    }
}

void PlayerDatabase::playerLogout(int networkID)
{
    Player *player = findPlayer(networkID);

    if (player)
    {
        player->mNetworkID = -1;
        player->mGameObject->kill();
        player->mGameObject = NULL;

        mActivePlayers--;
    }
}

void PlayerDatabase::spawnPlayer(Player *player)
{
    player->mGameObject = SceneManager::get()->createGameObject();
    player->mGameObject->addComponent(new SpriteComponent(player->mGameObject, "sprite", "Content/Textures/averysprite.png", 36, 6));
    player->mGameObject->addComponent(new RigidBodyComponent(player->mGameObject, "body", "Content/beingBody.txt"));
    player->mGameObject->addComponent(new HeroControlComponent(player->mGameObject, "control", player->mNetworkID));
    player->mGameObject->addComponent(new DragComponent(player->mGameObject, "drag"));
    player->mGameObject->setPosition(player->mSpawnPosition);
    player->mGameObject->getComponent<SpriteComponent>()->setAnimDelay(100);
    player->mGameObject->getComponent<RigidBodyComponent>()->getBody()->SetFixedRotation(true);
    player->mGameObject->getComponent<RigidBodyComponent>()->setCollisionGroup(1);
}

Player *PlayerDatabase::findPlayer(std::string name)
{
    for (unsigned int i = 0; i < mPlayers.size(); i++)
    {
        if (mPlayers[i]->mName == name) // Found the player!
            return mPlayers[i];
    }

    return NULL; // Couldn't find the player
}

Player *PlayerDatabase::findPlayer(int networkID)
{
    if (networkID <= 0) // Invalid
        return NULL;

    for (unsigned int i = 0; i < mPlayers.size(); i++)
    {
        if (mPlayers[i]->mNetworkID == networkID) // Found the player!
            return mPlayers[i];
    }

    return NULL; // Couldn't find the player
}
