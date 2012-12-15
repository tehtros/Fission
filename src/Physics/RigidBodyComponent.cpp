#include "Physics/RigidBodyComponent.h"

#include <fstream>
#include <Core/Math.h>
#include <Core/GameObject.h>
#include <Physics/PhysicsManager.h>

RigidBodyComponent::RigidBodyComponent(GameObject *object, std::string name, std::string bodyFile) : Component(object, name)
{
    //load the body file
    std::ifstream file(bodyFile.c_str(), std::ios::in);

    b2BodyDef def;

    int bodyType;
    file >> bodyType;
    def.type = (b2BodyType)bodyType;
    def.fixedRotation = false;

    mBody = PhysicsManager::get()->getWorld()->CreateBody(&def);
    mBody->SetUserData(mGameObject); //set the user data to this component's object

    //get the fixture count
    int fixtureCount = 0;
    file >> fixtureCount;

    for (int f = 0; f < fixtureCount; f++)
    {
        b2FixtureDef fixtureDef;
        b2Shape *shape;
        int isSensor;

        file >> fixtureDef.density;
        file >> fixtureDef.friction;
        file >> fixtureDef.restitution;
        file >> isSensor;
        fixtureDef.isSensor = isSensor;
        int shapeType;
        file >> shapeType;
        if (shapeType == b2Shape::e_circle)
        {
            b2CircleShape* circleShape = new b2CircleShape;
            shape = circleShape;
            file >> circleShape->m_radius;
        }
        else if (shapeType == b2Shape::e_polygon)
        {
            b2PolygonShape *polygonShape = new b2PolygonShape;
            shape = polygonShape;
            int vertexCount;
            file >> vertexCount;
            b2Vec2 *vecs = new b2Vec2[vertexCount];
            for (int v = 0; v < vertexCount; v++)
            {
                file >> vecs[v].x >> vecs[v].y;
            }
            polygonShape->Set(vecs, vertexCount);
            delete[] vecs;
        }
        fixtureDef.shape = shape;
        mBody->CreateFixture(&fixtureDef);
        delete shape;
    }
}

RigidBodyComponent::~RigidBodyComponent()
{
    //dtor
}

bool RigidBodyComponent::update(float dt)
{
    mGameObject->setPosition(sf::Vector2f(mBody->GetPosition().x,mBody->GetPosition().y), this);
    mGameObject->setRotation(radToDeg(mBody->GetAngle()), this);
    return true;
}

void RigidBodyComponent::onSetPosition(sf::Vector2f position)
{
    mBody->SetTransform(b2Vec2(position.x, position.y), mBody->GetAngle());
}

void RigidBodyComponent::onSetRotation(float rotation)
{
    mBody->SetTransform(mBody->GetPosition(), degToRad(rotation));
}
