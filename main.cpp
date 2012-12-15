#include <iostream>

#include <assert.h>
#include <Fission.h>

#include "PlayerControlComponent.h"

class GameState : public State
{
    public:
        GameState(Game *game)
        {
            mGame = game;
        }

        virtual void initialize()
        {
            // Tiling background
            background = new sf::Sprite(*ResourceManager::get()->getTexture("Content/Textures/background1.png"));
            mid = new sf::Sprite(*ResourceManager::get()->getTexture("Content/Textures/mid1.png"));

            GameObject *ground = SceneManager::get()->createGameObject();
            ground->addComponent(new GlowingShapeComponent(ground, "shape", "Content/groundShape.txt"));
            ground->addComponent(new RigidBodyComponent(ground, "body", "Content/groundBody.txt"));
            ground->setPosition(sf::Vector2f(7,-16));

            PhysicsManager::get()->setGroundBody(ground->getComponent<RigidBodyComponent>()->getBody());

            GameObject *object = SceneManager::get()->createGameObject();
            object->addComponent(new GlowingShapeComponent(object, "shape", "Content/testShape.txt", sf::Vector2f(1.1,1.1)));
            object->addComponent(new RigidBodyComponent(object, "body", "Content/testBody.txt"));
            object->addComponent(new DragComponent(object, "drag"));
            object->setPosition(sf::Vector2f(10,-8));
            //object->setRotation(30);

            GameObject *being = SceneManager::get()->createGameObject();
            being->addComponent(new SpriteComponent(being, "sprite", "Content/Textures/him.png", 36, 6));
            being->addComponent(new RigidBodyComponent(being, "body", "Content/beingBody.txt"));
            being->addComponent(new PlayerControlComponent(being, "control"));
            being->setPosition(sf::Vector2f(10,-12));

            being->getComponent<SpriteComponent>()->setAnimDelay(70);
            being->getComponent<RigidBodyComponent>()->getBody()->SetFixedRotation(true);

            // Create a light
            testLight = new ltbl::Light_Point();
            testLight->m_intensity = 5.0f;
            testLight->m_center = Vec2f(200.0f, 200.0f);
            testLight->m_radius = 1000.0f;
            testLight->m_size = 15.0f;
            testLight->m_spreadAngle = ltbl::pifTimes2;
            testLight->m_softSpreadAngle = 0.0f;
            testLight->CalculateAABB();

            testLight->m_bleed = 0.1f;
            testLight->m_linearizeFactor = 0.1f;

            RenderingManager::get()->getLightSystem()->AddLight(testLight);

            testLight->SetAlwaysUpdate(true);

            // Create an emissive light
            ltbl::EmissiveLight* emissiveLight = new ltbl::EmissiveLight();

            sf::Texture *text = new sf::Texture;

            if(!text->loadFromFile("Content/Textures/whiteEmissive.png"))
                abort();

            emissiveLight->SetTexture(text);

            emissiveLight->SetRotation(45.0f);

            emissiveLight->m_intensity = 1.1f;

            RenderingManager::get()->getLightSystem()->AddEmissiveLight(emissiveLight);

            emissiveLight->SetCenter(Vec2f(500.0f, 500.0f));

            RenderingManager::get()->getLightSystem()->m_ambientColor = sf::Color(150,150,150);
        }

        virtual bool update(float dt)
        {
            sf::Vector2i mousePos = sf::Mouse::getPosition(*RenderingManager::get()->getRenderWindow());
            testLight->SetCenter(Vec2f(static_cast<float>(mousePos.x), static_cast<float>(600) - static_cast<float>(mousePos.y)));

            return State::update(dt);
        }

        virtual void onPreRender(sf::RenderTarget *target, sf::RenderStates states)
        {
            target->draw(*background);
            target->draw(*mid);
        }

        virtual void onPostRender(sf::RenderTarget *target, sf::RenderStates states)
        {
        }

        Game *mGame;
        sf::Sprite *background;
        sf::Sprite *mid;
        ltbl::Light_Point* testLight;
};

int main()
{
    Game *game = new Game;
    game->run(new GameState(game));

    return 0;
}
