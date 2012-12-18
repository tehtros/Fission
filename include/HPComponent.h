#ifndef HPCOMPONENT_H
#define HPCOMPONENT_H

#include <Core/Component.h>

class HPComponent : public Component
{
    public:
        HPComponent(GameObject *object, std::string name);
        virtual ~HPComponent();

        void damage(int dmg){mHP-=dmg;}

        // Acessors
        int getHP(){return mHP;}

        // Mutators
        void setHP(int hp){mHP=hp;}

    protected:
        int mHP;
    private:
};

#endif // HPCOMPONENT_H
