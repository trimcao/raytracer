#include <iostream>
#include <exception>
#include <stdexcept>
#include <cmath>
#include <memory>
#include <limits>
#include <algorithm>
#include "include/Groups.h"
#include "include/Ray.h"
#include "include/Intersection.h"
#include "include/Util.h"
#include "include/Sphere.h"
#include "include/Transformations.h"
#include "include/Functions.h"

Groups::Groups(int ID)
{
    Transform = Matrix::Identity();
    TransformInverse = Matrix::Identity();
    Origin = Point(0., 0., 0.);
    AMaterial = Material();
    UseShadow = true;
    this->ID = ID;

    Shapes = std::vector<std::shared_ptr<Object>>();
    Parent = nullptr;
}

Groups::Groups() : Groups(0)
{
}

int Groups::GetID()
{
    return ID;
}

void Groups::AddChild(std::shared_ptr<Object> &S)
{
    S->SetParent(this);
    Shapes.push_back(S);
}

std::vector<Intersection<Object>> Groups::LocalIntersect(const Ray &LocalRay)
{
    std::vector<Intersection<Object>> Intersections;

    for (auto &S : Shapes)
    {
        auto XS = S->Intersect(LocalRay);
        Intersections.insert(Intersections.end(), XS.begin(), XS.end());
    }

    std::sort(Intersections.begin(), Intersections.end());

    return Intersections;
}

bool Groups::Include(Object *S)
{
    for (auto &Child: Shapes)
    {
        if (Child->Include(S))
            return true;
    }
    return false;
}

TEST_CASE("Creating a new group")
{
    Groups G;
    CHECK(G.GetTransform() == Matrix::Identity());
    CHECK(G.GetShapes().size() == 0);
}

TEST_CASE("Adding a child to a group")
{
    auto G = std::make_shared<Groups>(Groups());
    std::shared_ptr<Object> S = std::make_shared<TestShape>(TestShape());
    G->AddChild(S);
    CHECK(G->GetShapes()[0] == S);
    CHECK(S->GetParent() == G.get());
}

TEST_CASE("Intersecting a ray with an empty group")
{
    auto G = std::make_shared<Groups>(Groups());
    auto GPtr = std::shared_ptr<Object>(G);
    Ray R(Point(0., 0., 0.), Vector(0., 0., 1.));
    auto XS = G->LocalIntersect(R);

    CHECK(XS.size() == 0);
}

TEST_CASE("Intersecting a ray with a nonempty group")
{
    auto G = std::make_shared<Groups>(Groups());
    auto GPtr = std::shared_ptr<Object>(G);
    std::shared_ptr<Object> S1 = std::make_shared<Sphere>(Sphere());
    std::shared_ptr<Object> S2 = std::make_shared<Sphere>(Sphere());
    std::shared_ptr<Object> S3 = std::make_shared<Sphere>(Sphere());

    S2->SetTransform(Transformations::Translation(0., 0., -3.));
    S3->SetTransform(Transformations::Translation(5., 0., 0.));

    G->AddChild(S1);
    G->AddChild(S2);
    G->AddChild(S3);

    Ray R(Point(0., 0., -5.), Vector(0., 0., 1.));
    auto XS = G->LocalIntersect(R);

    auto Children = G->GetShapes();

    CHECK(XS.size() == 4);
    CHECK(XS[0].GetObject() == S2.get());
    CHECK(XS[1].GetObject() == S2.get());
    CHECK(XS[2].GetObject() == S1.get());
    CHECK(XS[3].GetObject() == S1.get());
}

TEST_CASE("Intersecting a transformed group")
{
    auto G = std::make_shared<Groups>(Groups());
    auto GPtr = std::shared_ptr<Object>(G);
    G->SetTransform(Transformations::Scaling(2., 2., 2.));

    std::shared_ptr<Object> S = std::make_shared<Sphere>(Sphere());
    S->SetTransform(Transformations::Translation(5., 0., 0.));

    G->AddChild(S);

    Ray R(Point(10., 0., -10.), Vector(0., 0., 1.));
    auto XS = G->Intersect(R);

    CHECK(XS.size() == 2);
}

TEST_CASE("Converting a point from world to object space")
{
    std::shared_ptr<Object> G1 = std::make_shared<Groups>(Groups());
    G1->SetTransform(Transformations::RotationY(M_PI/2));
    std::shared_ptr<Object> G2 = std::make_shared<Groups>(Groups());
    G2->SetTransform(Transformations::Scaling(2., 2., 2.));
    std::shared_ptr<Object> S = std::make_shared<Sphere>(Sphere());
    S->SetTransform(Transformations::Translation(5., 0., 0.));

    G1->AddChild(G2);
    G2->AddChild(S);

    auto P = TRay::WorldToObject(S.get(), Point(-2., 0., -10.));
    CHECK(P == Point(0., 0., -1.));
}

TEST_CASE("Converting a normal from object to world space")
{
    std::shared_ptr<Object> G1 = std::make_shared<Groups>(Groups());
    G1->SetTransform(Transformations::RotationY(M_PI/2));
    std::shared_ptr<Object> G2 = std::make_shared<Groups>(Groups());
    G2->SetTransform(Transformations::Scaling(1., 2., 3.));
    std::shared_ptr<Object> S = std::make_shared<Sphere>(Sphere());
    S->SetTransform(Transformations::Translation(5., 0., 0.));

    G1->AddChild(G2);
    G2->AddChild(S);

    auto N = TRay::NormalToWorld(S.get(), Vector(std::sqrt(3.)/3., std::sqrt(3.)/3, std::sqrt(3.)/3));
    CHECK(N == Vector(0.28571, 0.42857, -0.85714));
}

TEST_CASE("Finding the normal on a child object")
{
    std::shared_ptr<Object> G1 = std::make_shared<Groups>(Groups());
    G1->SetTransform(Transformations::RotationY(M_PI/2));
    std::shared_ptr<Object> G2 = std::make_shared<Groups>(Groups());
    G2->SetTransform(Transformations::Scaling(1., 2., 3.));
    std::shared_ptr<Object> S = std::make_shared<Sphere>(Sphere());
    S->SetTransform(Transformations::Translation(5., 0., 0.));

    G1->AddChild(G2);
    G2->AddChild(S);

    auto N = S->NormalAt(Point(1.7321, 1.1547, -5.5774));
    CHECK(N == Vector(0.2857, 0.42854, -0.85716));
}