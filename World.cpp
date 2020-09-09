#include "include/World.h"
#include "include/Sphere.h"

World::World(Light &NewLight, std::vector<std::shared_ptr<Object>> &NewObjects)
{
    ALight = std::make_shared<Light>(NewLight);
    Objects = NewObjects;
}

World::World(Light &&NewLight, std::vector<std::shared_ptr<Object>> &&NewObjects)
{
    World(NewLight, NewObjects);
}

World::World()
{
    ALight = nullptr;
    Objects = std::vector<std::shared_ptr<Object>>();
}

World World::DefaultWorld()
{
    Light L(Color(1.f, 1.f, 1.f), Point(-10.f, 10.f, -10.f));
    Sphere S1(1);
    S1.SetMaterial(Material(Color(0.8f, 1.f, 0.6f), 0.1f, 0.7f, 0.2f, 200.f));
    
    Sphere S2(2);
    S2.SetTransform(Matrix::Scaling(0.5f, 0.5f, 0.5f));

    World W;
    W.SetLight(L);
    W.AddObject<Sphere>(S1);
    W.AddObject<Sphere>(S2);

    return W;
}

TEST_CASE("Creating a world")
{
    World W;
    CHECK(W.GetObjects().size() == 0);
    CHECK(W.GetLight() == nullptr);
}

TEST_CASE("The default world")
{
    World W = World::DefaultWorld();
    CHECK(W.GetObjects().size() == 2);
    CHECK(W.GetLight()->GetIntensity() == Color(1.f, 1.f, 1.f) );
    CHECK(W.GetLight()->GetPosition() == Point(-10.f, 10.f, -10.f) );
}