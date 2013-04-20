#include "Rendering/SpriteComponent.h"

#include <SFML/Graphics/Sprite.hpp>
#include <iostream>

#include <Core/ResourceManager.h>
#include <Core/GameObject.h>
#include <Rendering/RenderingManager.h>

SpriteComponent::SpriteComponent(GameObject *object, std::string name, std::string imgPath, int frames, int framesPerRow) : Component(object, name)
{
    mTypeName = "SpriteComponent";

    mTexturePath = imgPath;

    if (mTexturePath == "") // If a path wasn't specified, we don't want to try to load anything
        return;

    sf::Texture *texture = getGame()->getResourceManager()->getTexture(mTexturePath);
    if (texture)
    {
        texture->setRepeated(false);
        mSprite = new sf::Sprite(*texture);
    }
    else
    {
        mSprite = NULL;
        return;
    }

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

    mRelativeRotation = 0;

    mKillOnAnimFinish = false;

    mTypeName = "SpriteComponent";
}

SpriteComponent::~SpriteComponent()
{
    //dtor
}

void SpriteComponent::serialize(sf::Packet &packet)
{
    Component::serialize(packet);

    packet << mTexturePath;
    packet << mCurFrame << mFrameDir << mAnimDelay << sf::Int8(mLoopAnim) << mFrames << mFramesPerRow << sf::Int8(mKillOnAnimFinish);
    packet << mRelativePosition.x << mRelativePosition.y << mRelativeRotation;
}

void SpriteComponent::deserialize(sf::Packet &packet)
{
    Component::deserialize(packet);

    packet >> mTexturePath;
    sf::Texture *texture = getGame()->getResourceManager()->getTexture(mTexturePath);
    if (texture)
    {
        texture->setRepeated(false);
        mSprite = new sf::Sprite(*texture);
    }
    else
    {
        mSprite = NULL;
        return;
    }

    //base animation stuff
    packet >> mCurFrame >> mFrameDir >> mAnimDelay;

    sf::Int8 loop;
    packet >> loop;
    mLoopAnim = loop;

    packet >> mFrames >> mFramesPerRow;

    mFrameDim = sf::Vector2i(texture->getSize().x/mFramesPerRow, texture->getSize().y/(mFrames/mFramesPerRow));
    mStartFrame = 0;
    mEndFrame = mFrames-1;

    sf::Int8 kill;
    packet >> kill;
    mKillOnAnimFinish = kill;

    packet >> mRelativePosition.x >> mRelativePosition.y >> mRelativeRotation;
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
    // Invalid frame - don't render
    if (!mSprite || !mEnabled || mCurFrame < mStartFrame || mCurFrame > mEndFrame)
        return;

    //calculate frame position
    int frameX = (mCurFrame%mFramesPerRow)*mFrameDim.x;
    int frameY = (mCurFrame/mFramesPerRow)*mFrameDim.y;

    mSprite->setTextureRect(sf::IntRect(frameX, frameY, mFrameDim.x, mFrameDim.y));

    //calculate pixel position from GameObject position
    sf::Vector2f newPos = ((mGameObject->getPosition()+mRelativePosition)*getGame()->getRenderingManager()->getPTU());
    newPos.y *= -1;
    newPos += getGame()->getRenderingManager()->getCameraScreenOffset();

    mSprite->setOrigin(sf::Vector2f(mFrameDim.x/2, mFrameDim.y/2));
    mSprite->setPosition(newPos);
    mSprite->setRotation(-mGameObject->getRotation()-mRelativeRotation);
    target->draw(*mSprite, states); //rendahhh!!!!
}

bool SpriteComponent::pointInSprite(sf::Vector2f point)
{
    int left = mGameObject->getPosition().x-((mSprite->getTextureRect().width/PTU)/2);
    int top = mGameObject->getPosition().y-((mSprite->getTextureRect().height/PTU)/2);
    int right = mGameObject->getPosition().x+((mSprite->getTextureRect().width/PTU)/2);
    int bottom = mGameObject->getPosition().y+((mSprite->getTextureRect().height/PTU)/2);

    if (point.x >= left && point.x <= right && point.y >= top && point.y <= bottom)
        return true;

    return false;
}

Component *SpriteComponent::createComponent(GameObject *object)
{
    return new SpriteComponent(object, "sprite", "", 1, 1);
}

void SpriteComponent::setTexture(sf::Texture *texture)
{
    if (mSprite)
        delete mSprite;

    if (texture)
    {
        texture->setRepeated(false);
        mSprite = new sf::Sprite(*texture);
    }
    else
        mSprite = NULL;
}

void SpriteComponent::setTexture(std::string path)
{
    setTexture(getGame()->getResourceManager()->getTexture(path));
}
