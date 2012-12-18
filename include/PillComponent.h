#ifndef PILLCOMPONENT_H
#define PILLCOMPONENT_H

#include <Core/Component.h>

class PillComponent : public Component
{
    public:
        PillComponent(GameObject *object, std::string name);
        virtual ~PillComponent();

        virtual bool update(float dt);
        virtual void onContactBegin(GameObject *object);

    protected:
        sf::Clock mLifeClock;

    private:
};

#endif // PILLCOMPONENT_H
