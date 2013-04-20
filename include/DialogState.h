#ifndef DIALOGUESTATE_H
#define DIALOGUESTATE_H

#include <Fission.h>

class DialogState : public State
{
    public:
        DialogState(Game *game, std::string text, int delay);
        virtual ~DialogState();

        virtual void initialize();

        virtual bool update(float dt);

        virtual void onPreRender(sf::RenderTarget *target, sf::RenderStates states);

        virtual void onPostRender(sf::RenderTarget *target, sf::RenderStates states);

    protected:
        std::vector <std::string> mPages;
        int mPage; // Current page to display
        int mCursor; // How much text has been typed
        int mDelay;

        sf::Sprite *mDialogBox;
        sf::Sprite *mGameBackground;

    private:
};

#endif // DIALOGUESTATE_H
