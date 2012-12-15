#include "Rendering/GlowingShapeComponent.h"

#include <SFML/Graphics.hpp>
#include <Core/ResourceManager.h>
#include <Core/GameObject.h>
#include <Rendering/RenderingManager.h>

GlowingShapeComponent::GlowingShapeComponent(GameObject *object, std::string name, std::string shapeFile, sf::Vector2f scale, bool castShadow, bool lit) :
    ShapeComponent(object, name, shapeFile, castShadow, lit)
{
    // Our blur shader
    sf::Shader blurShader;

    // Load the shader
    if (!blurShader.loadFromFile("Content/Shaders/blur.frag", sf::Shader::Fragment))
    {
        // TODO: Some kind of error message
    }
    blurShader.setParameter("texture", sf::Shader::CurrentTexture);
    blurShader.setParameter("blur_radius", 5.0f/mShape->getLocalBounds().width);

    // Render the shape to a texture
    sf::RenderTexture render;
    render.create(mShape->getLocalBounds().width + 128, mShape->getLocalBounds().height + 128);

    mShape->setPosition(sf::Vector2f(mShape->getLocalBounds().width/2 + 64, mShape->getLocalBounds().height/2 + 64)+
                        (mCenter*RenderingManager::get()->getPTU()));
    mShape->setScale(scale);
    render.draw(*mShape); //draw the shape
    render.display();

    //render that texture to another texture using a shader
    sf::Texture texture(render.getTexture());
    //sf::Texture texture(*ResourceManager::get()->getTexture("lesserbeing.png"));
    sf::Sprite tmpSprite(texture);

    sf::RenderStates renderStates = sf::RenderStates::Default;
    renderStates.shader = &blurShader;
    render.clear(sf::Color(0.0,0.0,0.0,0.0));
    render.draw(tmpSprite, renderStates);
    render.display();

    // Create an emissive light
    mEmissiveLight = new ltbl::EmissiveLight();

    mTexture = new sf::Texture(render.getTexture());
    mEmissiveLight->SetTexture(mTexture);

    mEmissiveLight->m_intensity = 1.1f;

    RenderingManager::get()->getLightSystem()->AddEmissiveLight(mEmissiveLight);

    mAlpha = 1.1f;
    mAlphaDir = -1.f;
}

GlowingShapeComponent::~GlowingShapeComponent()
{
    delete mTexture;
    delete mEmissiveLight;
}

bool GlowingShapeComponent::update(float dt)
{
    mAlpha += mAlphaDir*dt;

    if (mAlpha <= 0)
        mAlphaDir = 1.f;
    else if (mAlpha >= 1)
        mAlphaDir = -1.1f;

    mEmissiveLight->m_color = Color3f(mAlpha, mAlpha, mAlpha);

    return true;
}

void GlowingShapeComponent::onRender(sf::RenderTarget *target, sf::RenderStates states)
{
    sf::Vector2u screenSize = target->getSize();

    sf::Vector2f newPos = mGameObject->getPosition()*RenderingManager::get()->getPTU();
    newPos.y *= -1;

    Vec2f center(mCenter.x, mCenter.y);
    center.RotateBy(-mGameObject->getRotation());
    Vec2f lightPos = Vec2f(newPos.x, newPos.y)-(center*RenderingManager::get()->getPTU());

    mEmissiveLight->SetCenter(Vec2f(lightPos.x, screenSize.y-lightPos.y));
    mEmissiveLight->SetRotation(mGameObject->getRotation());

    if (mLightHull)
    {
        mLightHull->SetWorldCenter(Vec2f(newPos.x, screenSize.y-newPos.y));
        if (mLightHull->GetRotation() != mGameObject->getRotation())
            mLightHull->SetRotation(mGameObject->getRotation());
    }
}
