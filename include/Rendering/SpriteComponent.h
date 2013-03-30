#ifndef SPRITECOMPONENT_H
#define SPRITECOMPONENT_H

#include <SFML/System/Time.hpp>
#include <SFML/Graphics/Sprite.hpp>

#include "Core/Component.h"

class SpriteComponent : public Component
{
    public:
        SpriteComponent(GameObject *object, std::string name, std::string imgPath, int frames = 1, int framesPerRow = 1);
        virtual ~SpriteComponent();

        virtual void serialize(sf::Packet &packet);
        virtual void deserialize(sf::Packet &packet);

        virtual bool update(float dt);
        virtual void onRender(sf::RenderTarget *target, sf::RenderStates states = sf::RenderStates::Default);

        static Component *createComponent(GameObject *object);

        //accessors
        sf::Sprite *getSprite(){return mSprite;}
        sf::Vector2f getFrameSize(){return sf::Vector2f(mFrameDim.x, mFrameDim.y);}
        sf::Vector2f getPosition(){return mRelativePosition;}
        float getRotation(){return mRelativeRotation;}

        bool getAnimFinished(){return !mLoopAnim&&mCurFrame==mEndFrame;}

        //mutators
        void setFrames(int frames){mFrames = frames;}
        void setFramesPerRow(int frames){mFramesPerRow = frames;}
        void setAnimDelay(int delay){mAnimDelay = delay;}
        void setLoopAnim(bool loop){mLoopAnim = loop;}
        void setFrameLoop(int start, int end)
        {
            if (mStartFrame != start || mEndFrame != end) //make sure the frame loop is different before we reset
                mCurFrame=start;
            mStartFrame=start;mEndFrame=end;
        }
        void setKillOnAnimFinish(bool kill){mKillOnAnimFinish=kill;}
        void setTexture(sf::Texture *texture);
        void setTexture(std::string path);
        void setPosition(sf::Vector2f pos){mRelativePosition=pos;}
        void setRotation(float rot){mRelativeRotation=rot;}

    protected:
        sf::Sprite *mSprite;
        std::string mTexturePath;

        int mFrames;                            //Total number of frames in animation
        int mFramesPerRow;                      //Number of frames per row in sprite sheet
        sf::Vector2i mFrameDim;                 //The dimensions of a frame on the sprite sheet
        int mCurFrame;                          //The current frame of the animation
        int mFrameDir;                          //Direction to play the animation in
        sf::Clock mAnimClock;                   //clock used for animation
        int mAnimDelay;
        bool mLoopAnim;                         //Whether or not to loop the animation
        int mStartFrame;                        //The start frame of the animation loop
        int mEndFrame;                          //The end frame of the animation loop

        /// Position relative to the GameObject
        sf::Vector2f mRelativePosition;

        /// Rotation relative to the GameObject
        float mRelativeRotation;

        bool mKillOnAnimFinish;                 //Kill when the animation finishes?

    private:
};

#endif // SPRITECOMPONENT_H
