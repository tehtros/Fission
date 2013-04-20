#include "GUI/GUIManager.h"

GUIManager::GUIManager(Game *game) : Manager(game)
{
    mSFGUI = new sfg::SFGUI;
    mDesktop = new sfg::Desktop;
}

GUIManager::~GUIManager()
{
    delete mSFGUI;
}

bool GUIManager::update(float dt)
{
    mDesktop->Update(dt);

    return true;
}

void GUIManager::render(sf::RenderTarget *target, sf::RenderStates states)
{
    mSFGUI->Display(*target);
}
