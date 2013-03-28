#include "DialogState.h"

DialogState::DialogState(std::string text, int delay)
{
    mDelay = delay;
    mCursor = 0;
    mPage = 0;

    int i = 0;
    int lastnl = 0; // Last new line
    std::string page;
    int lines = 1;
    while ((int)text.size() > 0 && i < (int)text.size())
    {
        if (text[i] == '\n')
            lastnl = i;

        if ((text[i] == ' ' && i-lastnl > 65))
        {
            i--; // Take a step back
            while (text[i] != ' ' && i > 0) i--;
            text[i] = '\n';
            lastnl = i;
            lines++;

            if (lines > 5) // Finished this page, new page!
            {
                page.clear();
                for (int c = 0; c < i; c++)
                {
                    page += text[c];
                }
                text.replace(0, i+1, ""); // Erase it from the text
                mPages.push_back(page);
                lines = 0;
                i = 0;
                lastnl = 0;
            }
        }

        i++; // Next character
    }
    mPages.push_back(text);
}

DialogState::~DialogState()
{
    // Unpause the game when we are done with the dialogue
    SceneManager::get()->setPaused(false);
    PhysicsManager::get()->setPaused(false);

    mPages.clear();
    delete mDialogBox;
    delete mGameBackground;
}

void DialogState::initialize()
{
    mDialogBox = new sf::Sprite(*ResourceManager::get()->getTexture("Content/Textures/dialogbox.png"));
    mGameBackground = new sf::Sprite(*ResourceManager::get()->getTexture("Content/Textures/background.png"));

    mDialogBox->move(sf::Vector2f(0,640-150));

    SceneManager::get()->setPaused(true);
    PhysicsManager::get()->setPaused(true);
}

bool DialogState::update(float dt)
{
    if (InputManager::get()->getKeyState(sf::Keyboard::Return) == ButtonState::PRESSED)
        mPage++;

    if (mPage >= (int)mPages.size()) // Finished reading!
        return false;

    return true;
}

void DialogState::onPreRender(sf::RenderTarget *target, sf::RenderStates states)
{
    target->draw(*mGameBackground, states);
}

void DialogState::onPostRender(sf::RenderTarget *target, sf::RenderStates states)
{
    // Render the dialogue box
    target->draw(*mDialogBox, states);

    // Render the actual dialogue text
    sf::Text text(mPages[mPage], *ResourceManager::get()->getFont("Content/Fonts/font.ttf"), 16);
    text.setColor(sf::Color(50,50,50));
    text.move(sf::Vector2f(5, 640-150+5));
    target->draw(text, states);

    // Continue text
    sf::Text contText("Press enter to continue...", *ResourceManager::get()->getFont("Content/Fonts/font.ttf"), 16);
    contText.setColor(sf::Color(50,50,50));
    contText.move(sf::Vector2f(465, 580));
    target->draw(contText, states);
}
