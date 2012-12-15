#include "Rendering/ShapeComponent.h"

#include <fstream>
#include <Core/Math.h>
#include <Core/GameObject.h>
#include <Rendering/RenderingManager.h>

ShapeComponent::ShapeComponent(GameObject *object, std::string name, std::string shapeFile, bool castShadow, bool lit) : Component(object, name)
{
    mShape = new sf::ConvexShape; //allocate the shape
    mLightHull = NULL;

    mShape->setFillColor(sf::Color(255,0,0));

    //load the shape
    int index = 0;
    int indices;
    std::ifstream file(shapeFile.c_str(), std::ios::in);

    file >> indices;
    ((sf::ConvexShape*)mShape)->setPointCount(indices);

    //mins and maxes for calculating center of the shape
    sf::Vector2f min;
    sf::Vector2f max;

    while (index < indices)
    {
        float x,y;
        file >> x >> y;
        if (x < min.x) min.x = x;
        if (y < min.y) min.y = y;
        if (x > max.x) max.x = x;
        if (y > max.y) max.y = y;
        ((sf::ConvexShape*)mShape)->setPoint(index++, sf::Vector2f(x,-y)*RenderingManager::get()->getPTU());
    }

    mCenter = (min+max)/2.f;

    file.close();

    if (castShadow)
    {
        // Create a hull by loading it from a file
        mLightHull = new ltbl::ConvexHull();

        if(!mLightHull->LoadShape(shapeFile.c_str(), RenderingManager::get()->getPTU()))
            abort();

        // Pre-calculate certain aspects
        mLightHull->CalculateNormals();
        mLightHull->CalculateAABB();

        mLightHull->m_renderLightOverHull = lit;

        RenderingManager::get()->getLightSystem()->AddConvexHull(mLightHull);
    }
}

ShapeComponent::~ShapeComponent()
{
    delete mShape;
}

bool ShapeComponent::update(float dt)
{
    return true;
}

void ShapeComponent::onRender(sf::RenderTarget *target, sf::RenderStates states)
{
    sf::Vector2u screenSize = target->getSize();

    sf::Vector2f newPos = mGameObject->getPosition()*RenderingManager::get()->getPTU();
    newPos.y *= -1;

    mShape->setPosition(newPos);
    mShape->setRotation(-mGameObject->getRotation());

    if (mLightHull)
    {
        mLightHull->SetWorldCenter(Vec2f(mShape->getPosition().x, screenSize.y-mShape->getPosition().y));
        if (mLightHull->GetRotation() != mGameObject->getRotation())
            mLightHull->SetRotation(mGameObject->getRotation());
    }

    target->draw(*mShape, states);
}
