#include "include/World.h"
#include "include/Sphere.h"
#include "include/Intersection.h"
#include "include/Object.h"

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
    Light L(Color(1., 1., 1.f), Point(-10., 10., -10.f));
    Sphere S1(1);
    S1.SetMaterial(Material(Color(0.8, 1., 0.6f), 0.1, 0.7, 0.2, 200.f));
    
    Sphere S2(2);
    S2.SetTransform(Matrix::Scaling(0.5, 0.5, 0.5f));

    World W;
    W.SetLight(L);
    W.AddObject<Sphere>(S1);
    W.AddObject<Sphere>(S2);

    return W;
}

bool World::IsShadowed(Point &P)
{
    if (!ALight)
        return false;

    auto Vec = ALight->GetPosition() - P;
    auto Distance = Vec.Magnitude();
    auto Direction = Vec.Normalize();

    Ray R(P, Direction);
    auto Intersections = Intersect(R, *this);
    auto AHit = Hit(Intersections);

    if (AHit && AHit->GetT() < Distance)
    {
        return true;
    }

    return false;
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
    CHECK(W.GetLight()->GetIntensity() == Color(1., 1., 1.f) );
    CHECK(W.GetLight()->GetPosition() == Point(-10., 10., -10.f) );
}

TEST_CASE("There is no shadow when nothing is collinear with point and light")
{
    World W = World::DefaultWorld();
    Point P(0., 10., 0.f);
    CHECK(W.IsShadowed(P) == false);
}

TEST_CASE("The shadow when an object is between the point and the light")
{
    World W = World::DefaultWorld();
    Point P(10., -10., 10.f);
    CHECK(W.IsShadowed(P) == true);
}

TEST_CASE("There is no shadow when an object is behind the light")
{
    World W = World::DefaultWorld();
    Point P(-20., 20., -20.f);
    CHECK(W.IsShadowed(P) == false);
}

TEST_CASE("There is no shadow when an object is behind the point")
{
    World W = World::DefaultWorld();
    Point P(-2., 2., -2.f);
    CHECK(W.IsShadowed(P) == false);
}

TEST_CASE("ShadeHit() is given an intersection in shadow")
{
    World W;
    Light L(Color(1., 1., 1.f), Point(0., 0., -10.f));
    W.SetLight(L);

    Sphere S1;
    W.AddObject(S1);
    Sphere S2;
    S2.SetTransform(Matrix::Translation(0., 0., 10.f));
    W.AddObject(S2);

    Ray R(Point(0., 0., 5.f), Vector(0., 0., 1.f));
    Intersection<Object> I(4., S2);
    auto Comps = I.PrepareComputations(R);

    auto C = ShadeHit(W, Comps);

    CHECK(C == Color(0.1, 0.1, 0.1f));
}