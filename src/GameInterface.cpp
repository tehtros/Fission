#include "GameInterface.h"

#include <sstream>

#include "GameState.h"
#include "StructureManager.h"

GameInterface::GameInterface()
{
    mHasFocus = false;

    mPlaceStructure = NULL;

    int screenW = RenderingManager::get()->getRenderWindow()->getSize().x;
    int screenH = RenderingManager::get()->getRenderWindow()->getSize().y;

    GUIManager::get()->getDesktop()->SetProperty<std::string>( "*", "FontName", "Content/Fonts/font.ttf" );

    // Create the label.
	sfg::Label::Ptr label = sfg::Label::Create( "Hello world!" );
	sfg::Label::Ptr label2 = sfg::Label::Create( "Hi2" );

	// Create logout button
	mLogoutButton = sfg::Button::Create("Logout");
	mLogoutButton->GetSignal( sfg::Widget::OnLeftClick ).Connect(&GameInterface::onLogoutClick, this);
	mLogoutButton->SetPosition(sf::Vector2f(screenW-mLogoutButton->GetAllocation().width, 0));
	GUIManager::get()->getDesktop()->Add(mLogoutButton);

	// Create logout button
	mStructureButton = sfg::Button::Create("Structure");
	mStructureButton->GetSignal( sfg::Widget::OnLeftClick ).Connect(&GameInterface::onStructureButtonClick, this);
	mStructureButton->SetPosition(sf::Vector2f(screenW-mLogoutButton->GetAllocation().width-mStructureButton->GetAllocation().width, 0));
	GUIManager::get()->getDesktop()->Add(mStructureButton);

    // Create box for structure window layout
	mStructureBox = sfg::Box::Create( sfg::Box::HORIZONTAL, 5.0f );

	// Create left box for structure window layout
	mStructureLeftBox = sfg::Box::Create( sfg::Box::VERTICAL, 5.0f );
	mStructureLeftBox->Pack(label);

	// Create left box for structure window layout
	mStructureRightBox = sfg::Box::Create( sfg::Box::VERTICAL, 5.0f );
	mStructureRightBox->Pack(label2);

	// Create scrollers
	sfg::ScrolledWindow::Ptr left = sfg::ScrolledWindow::Create();
	left->SetRequisition( sf::Vector2f( 200, 160.f ) );
	left->SetScrollbarPolicy(sfg::ScrolledWindow::HORIZONTAL_AUTOMATIC | sfg::ScrolledWindow::VERTICAL_AUTOMATIC);
	left->SetPlacement( sfg::ScrolledWindow::TOP_LEFT );
	left->AddWithViewport(mStructureLeftBox);

	sfg::ScrolledWindow::Ptr right = sfg::ScrolledWindow::Create();
	right->SetRequisition( sf::Vector2f( 200, 160.f ) );
	right->SetScrollbarPolicy(sfg::ScrolledWindow::HORIZONTAL_AUTOMATIC | sfg::ScrolledWindow::VERTICAL_AUTOMATIC);
	right->SetPlacement( sfg::ScrolledWindow::TOP_LEFT );
	right->AddWithViewport(mStructureRightBox);

    mStructureBox->Pack(left);
    mStructureBox->Pack(right);

	// Create the structures window
	mStructureWindow = sfg::Window::Create();
	mStructureWindow->SetTitle("Structures");
	mStructureWindow->Add(mStructureBox);
	mStructureWindow->Show(false);

	GUIManager::get()->getDesktop()->Add(mStructureWindow);

	// Create the HUD sprite
	mHUDSprite = new sf::Sprite(*ResourceManager::get()->getTexture("Content/Textures/hud.png"));

    for (unsigned int i = 0; i < StructureManager::get()->getStructureTypes().size(); i++)
    {
        StructureType *type = StructureManager::get()->getStructureTypes()[i];

        sfg::Button::Ptr button = sfg::Button::Create(type->mName);
        mStructureLeftBox->Pack(button);
        mStructureButtons.push_back(button);
    }

    mCurrentStructureType = NULL;
}

GameInterface::~GameInterface()
{
    delete mHUDSprite;
    mStructureButtons.clear();
}

void GameInterface::update(float dt)
{
    for (unsigned int i = 0; i < mStructureButtons.size(); i++)
    {
        if (mStructureButtons[i]->GetState() == sfg::Widget::ACTIVE)
            buildStructurePage(StructureManager::get()->getStructureTypes()[i]);
    }

    updateStructurePlacement();

    if (mCurrentStructureType)
    {
        if (!mMetalCostGood && GameState::get()->getPlayer()->mMetal >= mCurrentStructureType->mMetal)
        {
            GUIManager::get()->getDesktop()->SetProperty("Label#strmetalcost", "Color", "#00ff00ff");
            mMetalCostGood = true;
        }
        else if (mMetalCostGood && GameState::get()->getPlayer()->mMetal < mCurrentStructureType->mMetal)
        {
            GUIManager::get()->getDesktop()->SetProperty("Label#strmetalcost", "Color", "#ff0000ff");
            mMetalCostGood = false;
        }

        if (!mEnergyCostGood && GameState::get()->getPlayer()->mEnergy >= mCurrentStructureType->mEnergy)
        {
            GUIManager::get()->getDesktop()->SetProperty("Label#strenergycost", "Color", "#00ff00ff");
            mEnergyCostGood = true;
        }
        else if (mEnergyCostGood && GameState::get()->getPlayer()->mEnergy < mCurrentStructureType->mEnergy)
        {
            GUIManager::get()->getDesktop()->SetProperty("Label#strenergycost", "Color", "#ff0000ff");
            mEnergyCostGood = false;
        }
    }
}

void GameInterface::render(sf::RenderTarget *target)
{
    int screenW = RenderingManager::get()->getRenderWindow()->getSize().x;
    int screenH = RenderingManager::get()->getRenderWindow()->getSize().y;

    mHUDSprite->setPosition(sf::Vector2f(0, screenH-mHUDSprite->getTexture()->getSize().y));
    target->draw(*mHUDSprite);

    // Render resources
    std::stringstream stream;
    std::string metalStr, energyStr;
    stream << GameState::get()->getPlayer()->mMetal << " " << GameState::get()->getPlayer()->mEnergy;
    stream >> metalStr >> energyStr;

    sf::Text metal(metalStr, *ResourceManager::get()->getFont("Content/Fonts/font.ttf"), 20);
    metal.setColor(sf::Color(255,0,0));
    metal.setPosition(sf::Vector2f(537, 84)+mHUDSprite->getPosition()-sf::Vector2f(metal.getLocalBounds().width/2, metal.getLocalBounds().height/2));
    target->draw(metal);

    sf::Text energy(energyStr, *ResourceManager::get()->getFont("Content/Fonts/font.ttf"), 20);
    energy.setColor(sf::Color(0,0,255));
    energy.setPosition(sf::Vector2f(700, 84)+mHUDSprite->getPosition()-sf::Vector2f(energy.getLocalBounds().width/2, energy.getLocalBounds().height/2));
    target->draw(energy);
}

void GameInterface::updateStructurePlacement()
{
    if (mPlaceStructure)
    {
        sf::Vector2f mousePos;
        mousePos.x = InputManager::get()->getMousePosition().x;
        mousePos.y = InputManager::get()->getMousePosition().y;
        mousePos = screenToWorld(mousePos-RenderingManager::get()->getCameraScreenOffset());
        mousePos.rotateBy(RenderingManager::get()->getCameraRotation(), RenderingManager::get()->getCameraPosition());

        mPlaceStructure->setPosition(mousePos);

        GameObject *planet = PlanetGenerator::get()->getPlanet(mPlaceStructure);
        sf::Vector2f distance = planet->getPosition()-mousePos;
        mPlaceStructure->setRotation(distance.getAngleTrig()+90);

        if (InputManager::get()->getKeyState(sf::Keyboard::Escape) == ButtonState::PRESSED)
        {
            mPlaceStructure->kill();
            mPlaceStructure = NULL;
        }

        if (InputManager::get()->getLMBState() == ButtonState::PRESSED) // Place the structure
        {
            sf::Packet packet;
            packet << PacketType::BUILD_STRUCTURE;
            packet << mCurrentStructureType->mTypeName << mPlaceStructure->getPosition().x << mPlaceStructure->getPosition().y
                   << mPlaceStructure->getRotation();
            NetworkManager::get()->send(packet);
            mPlaceStructure->kill();
            mPlaceStructure = NULL;
        }
    }
}

void GameInterface::buildStructurePage(StructureType *type)
{
    mStructureRightBox->RemoveAll();

    mCurrentStructureType = type;

    sfg::Label::Ptr title = sfg::Label::Create(type->mName);
    mStructureRightBox->Pack(title);

    sfg::Label::Ptr description = sfg::Label::Create(type->mDescription);
    description->SetRequisition(sf::Vector2f(300.f, 0.f));
    description->SetLineWrap(true);
    mStructureRightBox->Pack(description);

    sfg::Label::Ptr requirements = sfg::Label::Create("Requirements: ");
    mStructureRightBox->Pack(requirements);

    std::stringstream stream;
    std::string metalStr, energyStr;
    stream << type->mMetal << " " << type->mEnergy;
    stream >> metalStr >> energyStr;

    sfg::Label::Ptr metalLabel = sfg::Label::Create(std::string("Metal: ")+metalStr);
    metalLabel->SetId("strmetalcost");
    GUIManager::get()->getDesktop()->SetProperty("Label#strmetalcost", "Color", "#ff0000ff");
    mStructureRightBox->Pack(metalLabel);
    mMetalCostGood = false;

    sfg::Label::Ptr energyLabel = sfg::Label::Create(std::string("Energy: ")+energyStr);
    energyLabel->SetId("strenergycost");
    GUIManager::get()->getDesktop()->SetProperty("Label#strenergycost", "Color", "#ff0000ff");
    mStructureRightBox->Pack(energyLabel);
    mEnergyCostGood = false;

    sfg::Button::Ptr buildButton = sfg::Button::Create("Build");
	buildButton->GetSignal( sfg::Widget::OnLeftClick ).Connect(&GameInterface::onStructureBuildClick, this);
	mStructureRightBox->Pack(buildButton);

	mStructureWindow->RequestResize();
}

//**************************************************************************************************************
//**************************************************************************************************************
//**************************************************************************************************************

void GameInterface::onLogoutClick()
{
    sf::Packet logoutPacket;
    logoutPacket << PacketType::LOGOUT;
    NetworkManager::get()->send(logoutPacket);
}

void GameInterface::onStructureButtonClick()
{
    mStructureWindow->Show(!mStructureWindow->IsLocallyVisible());
}

void GameInterface::onStructureBuildClick()
{
    sf::Vector2f mousePos;
    mousePos.x = InputManager::get()->getMousePosition().x;
    mousePos.y = InputManager::get()->getMousePosition().y;
    mousePos = screenToWorld(mousePos-RenderingManager::get()->getCameraScreenOffset());
    mousePos.rotateBy(RenderingManager::get()->getCameraRotation(), RenderingManager::get()->getCameraPosition());

    mPlaceStructure = StructureManager::get()->createStructure(mCurrentStructureType->mTypeName, mousePos, 0);
}
