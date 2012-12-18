#include <iostream>

#include <assert.h>
#include <cstdlib>
#include <ctime>
#include <LTBL/Constructs/Vec2f.h>

#include "Villain.h"

class LoseState : public State
{
    public:
        LoseState(int score)
        {
            mScore = score;

            mLoseSprite = new sf::Sprite(*ResourceManager::get()->getTexture("Content/Textures/death.png"));
        }

        virtual void initialize()
        {
        }

        virtual bool update(float dt)
        {
            return State::update(dt);
        }

        virtual void onPreRender(sf::RenderTarget *target, sf::RenderStates states)
        {
        }

        virtual void onPostRender(sf::RenderTarget *target, sf::RenderStates states)
        {
            target->draw(*mLoseSprite, states);

            sf::Text score;
            score.setFont(*ResourceManager::get()->getFont("Content/Fonts/font.ttf"));
            score.setScale(sf::Vector2f(2.f, 2.f));

            char buffer[10];
            itoa(mScore, buffer, 10);
            score.setString(buffer);
            score.setPosition(sf::Vector2f(388,516));
            score.setOrigin(sf::Vector2f(score.getLocalBounds().width/2, score.getLocalBounds().height/2));

            target->draw(score);
        }

        int mScore;
        sf::Sprite *mLoseSprite;
};

class GameState : public State
{
    public:
        GameState(Game *game)
        {
            srand(time(NULL));
            mGame = game;
            pill = NULL;
        }

        virtual void initialize()
        {
            //Do loading screen
            sf::Sprite load(*ResourceManager::get()->getTexture("Content/Textures/load.png"));

            RenderingManager::get()->beginRender();
            RenderingManager::get()->get()->getRenderWindow()->draw(load);
            RenderingManager::get()->endRender();

            mScore = 0;
            mEnergy = 100.f;
            mMarineSpawnRate = 5000;
            mMechSpawnRate = 20000;

            ResourceManager::get()->getTexture("Content/Textures/explosion0.png");
            ResourceManager::get()->getTexture("Content/Textures/explosion1.png");
            ResourceManager::get()->getTexture("Content/Textures/death.png");

            mHPBackground = new sf::Sprite(*ResourceManager::get()->getTexture("Content/Textures/HUD/health.png"));
            mHP = new sf::Sprite(*ResourceManager::get()->getTexture("Content/Textures/HUD/hbar.png"));

            mStaminaBackground = new sf::Sprite(*ResourceManager::get()->getTexture("Content/Textures/HUD/stamina.png"));
            mStamina = new sf::Sprite(*ResourceManager::get()->getTexture("Content/Textures/HUD/sbar.png"));

            // Tiling background
            background = new sf::Sprite(*ResourceManager::get()->getTexture("Content/Textures/level.png"));
            mid = new sf::Sprite(*ResourceManager::get()->getTexture("Content/Textures/mid1.png"));

            GameObject *ground = SceneManager::get()->createGameObject();
            //ground->addComponent(new GlowingShapeComponent(ground, "shape", "Content/groundShape.txt"));
            ground->addComponent(new RigidBodyComponent(ground, "body", "Content/groundBody.txt"));
            ground->setPosition(sf::Vector2f(12.5,-18));
            ground->setType(GROUND);

            GameObject *lbound = SceneManager::get()->createGameObject();
            lbound->addComponent(new RigidBodyComponent(lbound, "body", "Content/boundBody.txt"));
            lbound->setPosition(sf::Vector2f(0,-9));
            lbound->setType(BOUND);

            GameObject *rbound = SceneManager::get()->createGameObject();
            rbound->addComponent(new RigidBodyComponent(rbound, "body", "Content/boundBody.txt"));
            rbound->setPosition(sf::Vector2f(25,-9));
            rbound->setType(BOUND);

            GameObject *platform1 = SceneManager::get()->createGameObject();
            //platform1->addComponent(new GlowingShapeComponent(platform1, "shape", "Content/platform1Shape.txt"));
            platform1->addComponent(new RigidBodyComponent(platform1, "body", "Content/platform1Body.txt"));
            platform1->setPosition(sf::Vector2f(2.5,-11.6));
            platform1->setType(PLATFORM);

            GameObject *platform2 = SceneManager::get()->createGameObject();
            //platform2->addComponent(new GlowingShapeComponent(platform2, "shape", "Content/platform2Shape.txt"));
            platform2->addComponent(new RigidBodyComponent(platform2, "body", "Content/platform2Body.txt"));
            platform2->setPosition(sf::Vector2f(14.8,-6));
            platform2->setType(PLATFORM);

            PhysicsManager::get()->setGroundBody(ground->getComponent<RigidBodyComponent>()->getBody());

            /*GameObject *object = SceneManager::get()->createGameObject();
            object->addComponent(new GlowingShapeComponent(object, "shape", "Content/testShape.txt", sf::Vector2f(1.1,1.1)));
            object->addComponent(new RigidBodyComponent(object, "body", "Content/testBody.txt"));
            object->addComponent(new DragComponent(object, "drag"));
            object->setPosition(sf::Vector2f(10,-8));*/

            hero = SceneManager::get()->createGameObject();
            hero->addComponent(new SpriteComponent(hero, "sprite", "Content/Textures/sprite.png", 36, 6));
            hero->addComponent(new RigidBodyComponent(hero, "body", "Content/badassBody.txt"));
            hero->addComponent(new PlayerControlComponent(hero, "control"));
            hero->addComponent(new DragComponent(hero, "drag"));
            hero->addComponent(new WeaponComponent(hero, "weapon"));
            hero->setPosition(sf::Vector2f(10,-12));
            hero->getComponent<SpriteComponent>()->setAnimDelay(100);
            hero->getComponent<RigidBodyComponent>()->getBody()->SetFixedRotation(true);
            hero->getComponent<WeaponComponent>()->setDamage(20);
            hero->getComponent<WeaponComponent>()->setRange(4.f);
            hero->getComponent<WeaponComponent>()->setVisibleBullets(false);
            hero->getComponent<HPComponent>()->setHP(2000.f);
            hero->setType(CHARACTER);
            hero->setTeam(0);

            // Create a light
            testLight = new ltbl::Light_Point();
            testLight->m_intensity = 5.0f;
            testLight->m_center = Vec2f(200.0f, 200.0f);
            testLight->m_radius = 200.0f;
            testLight->m_size = 15.0f;
            testLight->m_spreadAngle = PI/4;
            testLight->m_softSpreadAngle = 0.f;
            testLight->m_directionAngle = -PI/2;
            testLight->CalculateAABB();

            testLight->m_bleed = 0.1f;
            testLight->m_linearizeFactor = 0.1f;
            testLight->SetCenter(Vec2f(600, 600-400));

            RenderingManager::get()->getLightSystem()->AddLight(testLight);

            testLight->SetAlwaysUpdate(true);

            // Create a light
            ltbl::Light_Point *testLight2 = new ltbl::Light_Point();
            testLight2->m_intensity = 5.0f;
            testLight2->m_center = Vec2f(200.0f, 200.0f);
            testLight2->m_radius = 200.0f;
            testLight2->m_size = 15.0f;
            testLight2->m_spreadAngle = PI/4;
            testLight2->m_softSpreadAngle = 0.f;
            testLight2->m_directionAngle = -PI/2;
            testLight2->CalculateAABB();

            testLight2->m_bleed = 0.1f;
            testLight2->m_linearizeFactor = 0.1f;
            testLight2->SetCenter(Vec2f(200, 600-400));

            RenderingManager::get()->getLightSystem()->AddLight(testLight2);

            testLight2->SetAlwaysUpdate(true);

            // Ambient light
            RenderingManager::get()->getLightSystem()->m_ambientColor = sf::Color(100,100,100);
        }

        virtual bool update(float dt)
        {
            //sf::Vector2i mousePos = sf::Mouse::getPosition(*RenderingManager::get()->getRenderWindow());
            //testLight->SetCenter(Vec2f(static_cast<float>(mousePos.x), static_cast<float>(600) - static_cast<float>(mousePos.y)));

            if (hero->getComponent<HPComponent>()->getHP() <= 0)
                StateManager::get()->pushState(new LoseState(mScore));

            if (InputManager::get()->getRMBState() == ButtonState::PRESSED && mEnergy >= 30)
            {
                mEnergy -= 30;
                spawnExplosion1(screenToWorld(sf::Vector2f(InputManager::get()->getMousePosition().x, InputManager::get()->getMousePosition().y)));
            }
            else if (PhysicsManager::get()->getDragger() != NULL) // TK dragging stuff around
            {
                if (mEnergy <= 0)
                {
                    PhysicsManager::get()->getWorld()->DestroyJoint(PhysicsManager::get()->getDragger()->getMouseJoint());
                    PhysicsManager::get()->getDragger()->setMouseJoint(NULL);
                    PhysicsManager::get()->setDragger(NULL);
                }
                else
                    mEnergy -= 20.f*dt;
            }
            else
            {
                if (mEnergy < 100.f)
                    mEnergy += 20.f*dt;
                if (mEnergy > 100.f)
                    mEnergy = 100.f;
            }

            if (hero->getComponent<HPComponent>()->getHP() > 2000)
                hero->getComponent<HPComponent>()->setHP(2000);

            for (unsigned int g = 0; g < mGoodGuys.size(); g++)
            {
                if (mGoodGuys[g]->getComponent<HPComponent>()->getHP() <= 0)
                {
                    if (mMarineSpawnRate > 750)
                        mMarineSpawnRate -= 40;

                    if (mMechSpawnRate > 1000)
                        mMechSpawnRate -= 150;

                    switch (mGoodGuys[g]->getComponent<AIControlComponent>()->getAIType())
                    {
                        case MARINE:
                        {
                            mScore++;
                            break;
                        }
                        case MECH:
                        {
                            mScore += 4;
                            spawnExplosion2(mGoodGuys[g]->getPosition());
                            break;
                        }
                        case TANK:
                        {
                            mScore += 6;
                            break;
                        }
                    }

                    //DIE!!!
                    mGoodGuys[g]->kill();
                    mGoodGuys.erase(mGoodGuys.begin()+g);
                    g--;
                }
            }

            if (mMarineSpawnClock.getElapsedTime().asMilliseconds() > mMarineSpawnRate)
            {
                GameObject *enemy1 = spawnMarine(sf::Vector2f(-5,-15));
                GameObject *enemy2 = spawnMarine(sf::Vector2f(25,-15));
                GameObject *enemy3 = spawnMarine(sf::Vector2f(30,-15));
                enemy1->getComponent<AIControlComponent>()->setTarget(hero);
                enemy2->getComponent<AIControlComponent>()->setTarget(hero);
                enemy3->getComponent<AIControlComponent>()->setTarget(hero);
                mGoodGuys.push_back(enemy1);
                mGoodGuys.push_back(enemy2);
                mGoodGuys.push_back(enemy3);
                mMarineSpawnClock.restart();
            }

            if (mMechSpawnClock.getElapsedTime().asMilliseconds() > mMechSpawnRate)
            {
                GameObject *enemy = spawnMech(sf::Vector2f(30,-13));
                enemy->getComponent<AIControlComponent>()->setTarget(hero);
                mGoodGuys.push_back(enemy);
                mMechSpawnClock.restart();
            }

            if (mPlaneSpawnClock.getElapsedTime().asMilliseconds() > 3000)
            {
                GameObject *enemy = spawnAirPlane(sf::Vector2f(-5,-2.5));
                mPlaneSpawnClock.restart();
            }

            if (mPillSpawnClock.getElapsedTime().asMilliseconds() > 20000)
            {
                pill = spawnPill(sf::Vector2f(14.8,-4));
                mPillSpawnClock.restart();
            }

            return State::update(dt);
        }

        virtual void onPreRender(sf::RenderTarget *target, sf::RenderStates states)
        {
            target->draw(*background);
            //target->draw(*mid);
        }

        virtual void onPostRender(sf::RenderTarget *target, sf::RenderStates states)
        {
            sf::Text score;
            score.setFont(*ResourceManager::get()->getFont("Content/Fonts/font.ttf"));

            char buffer[10];
            itoa(mScore, buffer, 10);
            score.setPosition(sf::Vector2f(10,10));
            score.setString(std::string("Score: ")+buffer);
            target->draw(score);

            float hp = hero->getComponent<HPComponent>()->getHP();
            mHP->setTextureRect(sf::IntRect(sf::Vector2i(0, 0), sf::Vector2i(170-(((2000.f-hp)/2000.f)*145.f),50)));
            mHP->setPosition(sf::Vector2f(400,5));
            target->draw(*mHP);

            mHPBackground->setPosition(sf::Vector2f(400,5));
            target->draw(*mHPBackground);

            mStamina->setTextureRect(sf::IntRect(sf::Vector2i(0, 0), sf::Vector2i(170-(((100.f-mEnergy)/100.f)*145.f),50)));
            mStamina->setPosition(sf::Vector2f(600,5));
            target->draw(*mStamina);

            mStaminaBackground->setPosition(sf::Vector2f(600,5));
            target->draw(*mStaminaBackground);
        }

        Game *mGame;
        sf::Sprite *background;
        sf::Sprite *mid;
        ltbl::Light_Point* testLight;
        GameObject *hero; // Hahaha, not really
        std::vector <GameObject*> mGoodGuys;

        int mScore;

        sf::Clock mMarineSpawnClock;
        sf::Clock mMechSpawnClock;
        sf::Clock mPlaneSpawnClock;
        int mMarineSpawnRate;
        int mMechSpawnRate;

        sf::Clock mPillSpawnClock;
        GameObject *pill;

        sf::Sprite *mHPBackground;
        sf::Sprite *mHP;
        sf::Sprite *mStaminaBackground;
        sf::Sprite *mStamina;

        float mEnergy;
};

int main()
{
    Game *game = new Game;
    game->run(new GameState(game));

    return 0;
}
