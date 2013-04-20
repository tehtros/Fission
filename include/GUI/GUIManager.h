#ifndef GUIMANAGER_H
#define GUIMANAGER_H

#include <SFGUI/SFGUI.hpp>
#include <SFML/Graphics.hpp>

#include <Core/Manager.h>

class GUIManager : public Manager
{
    public:
        GUIManager(Game *game);
        virtual ~GUIManager();

        virtual bool update(float dt);
        void render(sf::RenderTarget *target, sf::RenderStates states = sf::RenderStates::Default);

        // Accessors
        sfg::SFGUI *getSFGUI(){return mSFGUI;}
        sfg::Desktop *getDesktop(){return mDesktop;}

    protected:
        sfg::SFGUI *mSFGUI;
        sfg::Desktop *mDesktop;

    private:
};

#endif // GUIMANAGER_H
