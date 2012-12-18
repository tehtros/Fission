#ifndef AICONTROLCOMPONENT_H
#define AICONTROLCOMPONENT_H

#include <SFML/Graphics/Sprite.hpp>
#include "HPComponent.h"

class AIControlComponent : public HPComponent
{
    public:
        AIControlComponent(GameObject *object, std::string name, std::string lArmTexture, std::string rArmTexture);
        virtual ~AIControlComponent();

        virtual bool update(float dt);
        virtual void onRender(sf::RenderTarget *target, sf::RenderStates states);

        virtual void onPreSolve(GameObject *object, b2Contact* contact, const b2Manifold* oldManifold);
        virtual void onContactBegin(GameObject *object);
        virtual void onContactEnd(GameObject *object);

        int getAIType(){return mAIType;}

        void setTarget(GameObject *target){mTarget=target;}
        void setAIType(int type){mAIType=type;}

    protected:
        GameObject *mTarget;
        int mMoveState;
        int mAimDirection;
        int mWalkDirection;
        bool mFire;

        bool mOnGround;
        int mContactCount;
        sf::Clock mContactClock; //time since last contact

        sf::Sprite *mLArmSprite; // Sprite for the left aimer arm
        sf::Sprite *mRArmSprite; // Sprite for the right aimer arm
        float mArmRotation;

        //How brave is he?
        float mMinDist;
        float mMaxDist;
        float mSpeed;

        int mAIType; // Marine,

    private:
};

#endif // AICONTROLCOMPONENT_H
