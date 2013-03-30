#ifndef PLANETGENERATOR_H
#define PLANETGENERATOR_H

#include <Fission.h>

class PlanetGenerator
{
    public:
        PlanetGenerator();
        virtual ~PlanetGenerator();

        GameObject *generatePlanet();

    protected:
        int mPlanetCount;

    private:
};

#endif // PLANETGENERATOR_H
