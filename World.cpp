#include "include/World.h"
#include "include/Object.h"
#include "include/Sphere.h"
#include "include/Intersection.h"

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
    Light L(Color(1., 1., 1.), Point(-10., 10., -10.));
    Sphere S1(1);
    S1.SetMaterial(Material(Color(0.8, 1., 0.6), 0.1, 0.7, 0.2, 200.));
    
    Sphere S2(2);
    S2.SetTransform(Matrix::Scaling(0.5, 0.5, 0.5));

    World W;
    W.SetLight(L);
    W.AddObject<Sphere>(S1);
    W.AddObject<Sphere>(S2);

    return W;
}

std::vector<Intersection<Object>> World::Intersect(const Ray &R)
{
    std::vector<Intersection<Object>> Intersections;

    // now all objects are sphere
    for (auto O : Objects)
    {
        auto XS = O->Intersect(R, O);

        Intersections.insert(Intersections.end(), XS.begin(), XS.end());
    }

    // sort the intersections
    std::sort(Intersections.begin(), Intersections.end());

    return Intersections;
}

Color World::ShadeHit(PreComputations<Object> &Comps, bool RenderShadow)
{
    if (!ALight)
        return Color(0., 0., 0.);

    bool IsInShadow = false;
    if (RenderShadow)
        IsInShadow = IsShadowed(Comps.OverPosition);

    return Lighting(Comps.AObject->GetMaterial(), *ALight, Comps.OverPosition, Comps.EyeV, Comps.NormalV, IsInShadow);
}

Color World::ColorAt(Ray &R, bool RenderShadow)
{
    auto Intersects = Intersect(R);
    auto H = Hit(Intersects);
    if (H != nullptr)
    {
        auto Comps = H->PrepareComputations(R);
        return ShadeHit(Comps, RenderShadow);
    }

    return Color(0., 0., 0.);
}

bool World::IsShadowed(Point &P)
{
    if (!ALight)
        return false;

    auto Vec = ALight->GetPosition() - P;
    auto Distance = Vec.Magnitude();
    auto Direction = Vec.Normalize();

    Ray R(P, Direction);
    auto Intersections = Intersect(R);
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
    CHECK(W.GetLight()->GetIntensity() == Color(1., 1., 1.) );
    CHECK(W.GetLight()->GetPosition() == Point(-10., 10., -10.) );
}

TEST_CASE("Intersect a world with a ray")
{
    World W = World::DefaultWorld();
    Ray R(Point(0., 0., -5.), Vector(0., 0., 1.));

    auto XS = W.Intersect(R);

    CHECK(Util::Equal(XS[0].GetT(), 4.));
    CHECK(Util::Equal(XS[1].GetT(), 4.5));
    CHECK(Util::Equal(XS[2].GetT(), 5.5));
    CHECK(Util::Equal(XS[3].GetT(), 6.));

    // the following value is 4 because two intersections keep a shared_ptr to the object
    // another owner is World
    // the last user is the pointer that prints use_count()
    // std::cout << "Object 1 num user: " << XS[1].GetObject().use_count() << '\n';
}

TEST_CASE("Precomputing the state of an intersection")
{
    Ray R(Point(0., 0., -5.), Vector(0., 0., 1.));
    Sphere Shape(1);
    auto I = Intersection<Sphere>(4., Shape);
    auto Comps = I.PrepareComputations(R);

    CHECK(Comps.AObject->GetID() == Shape.GetID());
    CHECK(Comps.Position == Point(0., 0., -1.));
    CHECK(Comps.EyeV == Vector(0., 0., -1.));
    CHECK(Comps.NormalV == Vector(0., 0., -1.));
}

TEST_CASE("The hit, when an intersection occurs on the outside")
{
    Ray R(Point(0., 0., -5.), Vector(0., 0., 1.));
    Sphere Shape(1);
    auto I = Intersection<Object>(4., Shape);
    auto Comps = I.PrepareComputations(R);

    CHECK(Comps.IsInside == false);
}

TEST_CASE("The hit, when an intersection occurs on the inside")
{
    Ray R(Point(0., 0., 0.), Vector(0., 0., 1.));
    Sphere Shape(1);
    auto I = Intersection<Sphere>(1., Shape);
    auto Comps = I.PrepareComputations(R);

    CHECK(Comps.Position == Point(0., 0., 1.));
    CHECK(Comps.EyeV == Vector(0., 0., -1.));
    CHECK(Comps.NormalV == Vector(0., 0., -1.));
    CHECK(Comps.IsInside == true);
}

TEST_CASE("Shading an intersection")
{
    auto W = World::DefaultWorld();
    Ray R(Point(0., 0., -5.), Vector(0., 0., 1.));
    auto Shape = W.GetObjectAt(0);
    auto I = Intersection<Object>(4., Shape);
    auto Comps = I.PrepareComputations(R);
    auto C = W.ShadeHit(Comps);

    CHECK(C == Color(0.38066, 0.47583, 0.2855));
}

TEST_CASE("Shading an intersection from the inside")
{
    auto W = World::DefaultWorld();
    W.SetLight(Light(Color(1., 1., 1.), Point(0., 0.25, 0.)));

    Ray R(Point(0., 0., 0.), Vector(0., 0., 1.));
    auto Shape = W.GetObjectAt(1);
    auto I = Intersection<Object>(0.5, Shape);
    auto Comps = I.PrepareComputations(R);
    
    bool IsInShadow = W.IsShadowed(Comps.OverPosition);

    auto C = W.ShadeHit(Comps);

    CHECK(C == Color(0.90498, 0.90498, 0.90498));
    CHECK(Comps.IsInside == true);
}

TEST_CASE("Smart pointer and polymorphism")
{
    std::vector<std::unique_ptr<Object>> X;
    Sphere S;
    S.SetTransform(Matrix::Scaling(0.5, 0.5, 0.5));
    auto V = S.NormalAt(Point(0.5, 0., 0.));

    // need to use make_unique, otherwise it will complain when free the memory
    X.push_back(std::make_unique<Sphere>(S));

    // std::cout << "Pointer location: " << &S << '\n';
    // std::cout << "Pointer location: " << X[0] << '\n';
    // std::cout << "Vector location: " << &X << '\n';
    // std::cout << "Normal vector value: " << V << '\n';
    // std::cout << "Normal vector value from vector X: " << X[0]->NormalAt(Point(0.5, 0., 0.)) << '\n';
}

TEST_CASE("The color when a ray misses")
{
    auto W = World::DefaultWorld();
    Ray R(Point(0., 0., -5.), Vector(0., 1., 0.));

    auto C = W.ColorAt(R, true);
    CHECK(C == Color(0., 0., 0.));
}

TEST_CASE("The color when a ray hits")
{
    auto W = World::DefaultWorld();
    Ray R(Point(0., 0., -5.), Vector(0., 0., 1.));

    auto C = W.ColorAt(R, true);
    CHECK(C == Color(0.38066, 0.47583, 0.2855));
}

TEST_CASE("The color with an intersection behind the ray")
{
    auto W = World::DefaultWorld();
    auto Outer = W.GetObjectAt(0);
    auto Mat = Outer->GetMaterial();
    Mat.SetAmbient(1.);
    Outer->SetMaterial(Mat);

    auto Inner = W.GetObjectAt(1);
    Mat = Inner->GetMaterial();
    Mat.SetAmbient(1.);
    Inner->SetMaterial(Mat);

    // std::cout << "Inner ambient:" << Inner->GetMaterial().GetAmbient() << '\n';

    Ray R(Point(0., 0., 0.75), Vector(0., 0., -1.));

    auto C = W.ColorAt(R, true);
    CHECK(C == Inner->GetMaterial().GetColor());
}

TEST_CASE("There is no shadow when nothing is collinear with point and light")
{
    World W = World::DefaultWorld();
    Point P(0., 10., 0.);
    CHECK(W.IsShadowed(P) == false);
}

TEST_CASE("The shadow when an object is between the point and the light")
{
    World W = World::DefaultWorld();
    Point P(10., -10., 10.);
    CHECK(W.IsShadowed(P) == true);
}

TEST_CASE("There is no shadow when an object is behind the light")
{
    World W = World::DefaultWorld();
    Point P(-20., 20., -20.);
    CHECK(W.IsShadowed(P) == false);
}

TEST_CASE("There is no shadow when an object is behind the point")
{
    World W = World::DefaultWorld();
    Point P(-2., 2., -2.);
    CHECK(W.IsShadowed(P) == false);
}

TEST_CASE("ShadeHit() is given an intersection in shadow")
{
    World W;
    Light L(Color(1., 1., 1.), Point(0., 0., -10.));
    W.SetLight(L);

    Sphere S1;
    W.AddObject(S1);
    Sphere S2;
    S2.SetTransform(Matrix::Translation(0., 0., 10.));
    W.AddObject(S2);

    Ray R(Point(0., 0., 5.), Vector(0., 0., 1.));
    Intersection<Object> I(4., S2);
    auto Comps = I.PrepareComputations(R);

    auto C = W.ShadeHit(Comps);

    CHECK(C == Color(0.1, 0.1, 0.1));
}