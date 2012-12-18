#include "Rendering/SpriteComponent.h"

#include <SFML/Graphics/Sprite.hpp>
#include <iostream>

#include <Core/ResourceManager.h>
#include <Core/GameObject.h>
#include <Rendering/RenderingManager.h>

SpriteComponent::SpriteComponent(GameObject *object, std::string name, std::string imgPath, int frames, int framesPerRow) : Component(object, name)
{
    sf::Texture *texture = ResourceManager::get()->getTexture(imgPath);
    texture->setRepeated(false);
    mSprite = new sf::Sprite(*texture);

    //base animation stuff
    mCurFrame = 0;
    mFrameDir = 1;
    mAnimDelay = 150;
    mLoopAnim = true;
    mFrames = frames;
    mFramesPerRow = framesPerRow;

    mFrameDim = sf::Vector2i(texture->getSize().x/mFramesPerRow, texture->getSize().y/(mFrames/mFramesPerRow));
    mStartFrame = 0;
    mEndFrame = mFrames-1;

    mKillOnAnimFinish = false;
}

SpriteComponent::~SpriteComponent()
{
    //dtor
}

bool SpriteComponent::update(float dt)
{
    if (mKillOnAnimFinish && getAnimFinished())
    {
        mGameObject->kill();
    }

    if (mAnimClock.getElapsedTime().asMilliseconds() >= mAnimDelay && (mLoopAnim || mCurFrame != mEndFrame))
    {
        mAnimClock.restart();
        mCurFrame += mFrameDir;

        if (mCurFrame > mEndFrame && mFrameDir == 1 && mLoopAnim)
            mCurFrame = mStartFrame;
        else if (mCurFrame < mStartFrame && mFrameDir == -1 && mLoopAnim)
            mCurFrame = mEndFrame;
    }

    return true;
}

void SpriteComponent::onRender(sf::RenderTarget *target, sf::RenderStates states)
{
    //invalid frame - don't render
    if (mCurFrame < mStartFrame || mCurFrame > mEndFrame)
        return;

    //calculate frame position
    int frameX = (mCurFrame%mFramesPerRow)*mFrameDim.x;
    int frameY = (mCurFrame/mFramesPerRow)*mFrameDim.y;

    mSprite->setTextureRect(sf::IntRect(frameX, frameY, mFrameDim.x, mFrameDim.y));

    //calculate pixel position from GameObject position
    sf::Vector2f newPos = mGameObject->getPosition()*RenderingManager::get()->getPTU();
    newPos.y *= -1;

    mSprite->setPosition(newPos-sf::Vector2f(mFrameDim.x/2, mFrameDim.y/2));
    mSprite->setRotation(-mGameObject->getRotation());
    target->draw(*mSprite, states); //rendahhh!!!!
}
