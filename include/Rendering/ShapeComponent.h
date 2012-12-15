#ifndef SHAPECOMPONENT_H
#define SHAPECOMPONENT_H

#include <SFML/Graphics/ConvexShape.hpp>
#include <LTBL/Light/ConvexHull.h>
#include <Core/Component.h>

class ShapeComponent : public Component
{
    public:
        ShapeComponent(GameObject *object, std::string name, std::string shapeFile, bool castShadow = true, bool lit = false);
        virtual ~ShapeComponent();

        virtual bool update(float dt);
        virtual void onRender(sf::RenderTarget *target, sf::RenderStates states = sf::RenderStates::Default);

    protected:
        sf::Shape *mShape;
        ltbl::ConvexHull *mLightHull;

        /// Pre-calculated center of the shape
        sf::Vector2f mCenter;

    private:
};

#endif // SHAPECOMPONENT_H
