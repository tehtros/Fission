#include "Physics/RigidBodyComponent.h"

#include <iostream>
#include <fstream>
#include <Core/Math.h>
#include <Core/GameObject.h>
#include <Physics/PhysicsManager.h>

RigidBodyComponent::RigidBodyComponent(GameObject *object, std::string name, std::string bodyFile) : Component(object, name)
{
    mCollisionGroup = 0; // Everything collides with everything to begin with
    mBody = NULL;
    mTypeName = "RigidBodyComponent";

    if (bodyFile == "") // If a path wasn't specified, we don't want to try to load anything
        return;

    //load the body file
    std::ifstream file(bodyFile.c_str(), std::ios::in);

    if (!file.is_open())
    {
        std::cout << "Error: Body file couldn't be loaded on " << object->getID() << std::endl;
        return;
    }

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
    PhysicsManager::get()->getWorld()->DestroyBody(mBody);
}

void RigidBodyComponent::serialize(sf::Packet &packet)
{
    Component::serialize(packet);

    packet << int(mBody->GetType());

    packet << mBody->GetPosition().x;
    packet << mBody->GetPosition().y;
    packet << mBody->GetAngle();

    packet << sf::Int8(mBody->IsFixedRotation());
    packet << sf::Int8(mBody->IsSleepingAllowed());

    //get the fixture count
    int fixtureCount = 0;
    for (b2Fixture* f = mBody->GetFixtureList(); f; f = f->GetNext())
        fixtureCount++;
    packet << fixtureCount;

    for (b2Fixture* f = mBody->GetFixtureList(); f; f = f->GetNext())
    {
        packet << f->GetDensity();
        packet << f->GetFriction();
        packet << sf::Int8(f->IsSensor());
        b2Shape::Type shapeType = f->GetType();
        packet << int(shapeType);
        if (shapeType == b2Shape::e_circle)
        {
            b2CircleShape* circleShape = (b2CircleShape*)f->GetShape();
            packet << circleShape->m_radius;
        }
        else if (shapeType == b2Shape::e_polygon)
        {
            b2PolygonShape* polygonShape = (b2PolygonShape*)f->GetShape();
            packet << polygonShape->GetVertexCount();
            for (int v = 0; v < polygonShape->GetVertexCount(); v++)
            {
                packet << polygonShape->GetVertex(v).x;
                packet << polygonShape->GetVertex(v).y;
            }
        }
    }
}

void RigidBodyComponent::deserialize(sf::Packet &packet)
{
    Component::deserialize(packet);

    b2BodyDef bodyDef;

    int bodyType;
    packet >> bodyType;
    bodyDef.type = (b2BodyType)bodyType;

    packet >> bodyDef.position.x;
    packet >> bodyDef.position.y;
    packet >> bodyDef.angle;

    sf::Int8 fixedRotation;
    sf::Int8 allowSleep;
    packet >> fixedRotation;
    packet >> allowSleep;
    bodyDef.fixedRotation = fixedRotation;
    bodyDef.allowSleep = allowSleep;

    mBody = PhysicsManager::get()->getWorld()->CreateBody(&bodyDef);
    mBody->SetUserData(mGameObject); //set the user data to this object

    //get the fixture count
    int fixtureCount = 0;
    packet >> fixtureCount;

    for (int f = 0; f < fixtureCount; f++)
    {
        b2FixtureDef fixtureDef;
        b2Shape *shape;
        sf::Int8 isSensor;
        packet >> fixtureDef.density;
        packet >> fixtureDef.friction;
        packet >> isSensor;
        fixtureDef.isSensor = isSensor;
        int shapeType;
        packet >> shapeType;
        if (shapeType == b2Shape::e_circle)
        {
            b2CircleShape* circleShape = new b2CircleShape;
            shape = circleShape;
            packet >> circleShape->m_radius;
        }
        else if (shapeType == b2Shape::e_polygon)
        {
            b2PolygonShape *polygonShape = new b2PolygonShape;
            shape = polygonShape;
            int vertexCount;
            packet >> vertexCount;
            b2Vec2 *vecs = new b2Vec2[vertexCount];
            for (int v = 0; v < vertexCount; v++)
            {
                packet >> vecs[v].x;
                packet >> vecs[v].y;
            }
            polygonShape->Set(vecs, vertexCount);
            delete[] vecs;
        }
        fixtureDef.shape = shape;
        mBody->CreateFixture(&fixtureDef);
        delete shape;
    }
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

Component *RigidBodyComponent::createComponent(GameObject *object)
{
    return new RigidBodyComponent(object, "body", "");
}
