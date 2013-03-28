#ifndef ENEMYCOMPONENT_H
#define ENEMYCOMPONENT_H

#include <Core/Component.h>

class GameState;

class EnemyComponent : public Component
{
    public:
        EnemyComponent(GameObject *object, std::string name, GameState *state);
        virtual ~EnemyComponent();

        virtual bool update(float dt);

        virtual void onContactBegin(GameObject *object);
        virtual void onContactEnd(GameObject *object);

    protected:
        int mMoveState;
        int mDirection;
        bool mOnGround;
        int mContactCount;
        sf::Clock mContactClock; //time since last contact

        GameObject *mLeader; // My leader to follow
        float mFollowDistance;

        GameState *mGameState;

    private:
};

#endif // ENEMYCOMPONENT_H
