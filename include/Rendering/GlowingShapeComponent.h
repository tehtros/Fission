#ifndef GLOWINGSHAPECOMPONENT_H
#define GLOWINGSHAPECOMPONENT_H

#include <LTBL/Light/EmissiveLight.h>

#include <Rendering/ShapeComponent.h>

class GlowingShapeComponent : public ShapeComponent
{
    public:
        GlowingShapeComponent(GameObject *object, std::string name, std::string shapeFile, sf::Vector2f scale = sf::Vector2f(1,1), bool castShadow = true, bool lit = false);
        virtual ~GlowingShapeComponent();

        virtual bool update(float dt);
        virtual void onRender(sf::RenderTarget *target, sf::RenderStates states = sf::RenderStates::Default);

    protected:
        sf::Texture *mTexture;
        ltbl::EmissiveLight* mEmissiveLight;
        float mAlpha;
        float mAlphaDir;
    private:
};

#endif // GLOWINGSHAPECOMPONENT_H
