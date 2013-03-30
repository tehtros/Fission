#include "PlanetGenerator.h"

#include "Box2D/ConvexDecomposition/b2Polygon.h"
#include "perlin/perlin.h"

PlanetGenerator::PlanetGenerator()
{
    mPlanetCount = 0;
}

PlanetGenerator::~PlanetGenerator()
{
    //dtor
}

GameObject *PlanetGenerator::generatePlanet()
{
    float minRadius, maxRadius, circumference;
    int imageWidth, imageHeight;
    float gravity, gravitationalRadius;

    minRadius = (rand()%30)+60;
    maxRadius = minRadius+5+(rand()%5);
    circumference = 2*PI*minRadius; //2*pi*r

    imageWidth = maxRadius*RenderingManager::get()->getPTU()*2;
    imageHeight = maxRadius*RenderingManager::get()->getPTU()*2;

    gravity = (minRadius/75.f)*10.f;
    gravitationalRadius = (minRadius/75.f)*100.f;

    int heightMapSize = circumference*2; //resolution of height map is 0.5 meters per pixel
    float *heightMap = new float[heightMapSize]; //allocate the height map
    b2Vec2 *vertices = new b2Vec2[heightMapSize]; //vertices in meters
    b2Vec2 *pvertices = new b2Vec2[heightMapSize]; //per pixel vertices

    //populate the heightmap using perlin noise
    float pmin = 1000, pmax = -1000;
    for (int i = 0; i < heightMapSize; i++)
    {
        heightMap[i] = PerlinNoise1D(i, 1.11, 0.08, 10);

        //get the max and min values of perlin noise so we can normalize it later
        if (heightMap[i] < pmin)
            pmin = heightMap[i];
        if (heightMap[i] > pmax)
            pmax = heightMap[i];
    }

    //create the polygon shape for this planet from the heightmap data
    for (int i = 0; i < heightMapSize; i++)
    {
        heightMap[i] += fabs(pmin); //make sure heightmap starts from 0
        heightMap[i] /= (pmax-pmin); //normalize the heightmap
        heightMap[i] *= (maxRadius-minRadius)-1; //convert heightmap to our coordinate system

        //create the polygon
        float rotation = (((float)i)/((float)heightMapSize))*360.f;
        vertices[i].x = cos(degToRad(rotation))*(minRadius+heightMap[i]);
        vertices[i].y = sin(degToRad(rotation))*(minRadius+heightMap[i]);
        pvertices[i].x = floorf((vertices[i].x+maxRadius)*RenderingManager::get()->getPTU());
        pvertices[i].y = imageHeight-(floorf((vertices[i].y+maxRadius)*RenderingManager::get()->getPTU()));
    }

    //create and fill the planet texture
    sf::Image *image = new sf::Image;
    image->create(imageWidth, imageHeight, sf::Color(0,0,0,0));

    int nodes, pixelY, i, j, swap;
    int *nodeX = new int[heightMapSize]; //create as many nodes as there are vertices

    //Loop through the rows of the image.
    for (pixelY = 0; pixelY < imageHeight; pixelY++)
    {
        //Build a list of nodes.
        nodes = 0;
        j = heightMapSize-1;
        for (i = 0; i < heightMapSize; i++)
        {
            if ((pvertices[i].y<(double) pixelY && pvertices[j].y>=(double) pixelY)
                || (pvertices[j].y<(double) pixelY && pvertices[i].y>=(double) pixelY))
            {
                nodeX[nodes++] = (int)(pvertices[i].x+(pixelY-pvertices[i].y)/(pvertices[j].y-pvertices[i].y)
                                       *(pvertices[j].x-pvertices[i].x));
            }
            j=i;
        }

        //Sort the nodes, via a simple “Bubble” sort.
        i=0;
        while (i < nodes-1)
        {
            if (nodeX[i]>nodeX[i+1])
            {
                swap = nodeX[i]; nodeX[i]=nodeX[i+1]; nodeX[i+1]=swap; if (i) i--;
            }
            else
                i++;
        }

        //  Fill the pixels between node pairs.
        for (i=0; i<nodes; i+=2)
        {
            if (nodeX[i]>=imageWidth)
                break;
            if (nodeX[i+1]> 0)
            {
                if (nodeX[i] < 0)
                    nodeX[i]=0;
                if (nodeX[i+1] > imageWidth)
                    nodeX[i+1]=imageWidth;
                for (j=nodeX[i]; j<nodeX[i+1]; j++)
                {
                    image->setPixel(j,pixelY, sf::Color(255,255,0,255));
                }
            }
        }
    }

    std::string textureName = std::string("planetImage")+".png";
    image->saveToFile(textureName);
    delete image;

    mPlanetCount++;

    //Create the physics body
    b2FixtureDef polyprot;
    polyprot.density = 1.0f;
    polyprot.friction = 0.3f;
    //polyprot.restitution = 0.2f;

    //create the polygon for polygon decomposition
    b2Polygon pgon(vertices,heightMapSize);

    //create the body
    b2BodyDef bd;
    bd.type = b2_staticBody;

    b2Body *body = PhysicsManager::get()->getWorld()->CreateBody(&bd);
    b2FixtureDef* deleteMe = DecomposeConvexAndAddTo(&pgon, body, &polyprot);
    delete[] deleteMe;

    // CREATE THE GAME OBJECT!!!!
    GameObject *planet = SceneManager::get()->createGameObject();
    planet->addComponent(new SpriteComponent(planet, "sprite", textureName));
    planet->addComponent(new RigidBodyComponent(planet, "body", body));

    // Finally, delete everything we don't need
    delete[] heightMap; //we don't need the height map anymore - deallocate it
    delete[] nodeX;
    delete[] vertices;
    delete[] pvertices;

    return planet;
}
