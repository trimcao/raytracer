#include <iostream>
#include <exception>
#include <stdexcept>
#include <cmath>
#include <memory>
#include <limits>
#include <algorithm>
#include "include/CSG.h"
#include "include/Ray.h"
#include "include/Intersection.h"
#include "include/Util.h"
#include "include/Sphere.h"
#include "include/Cubes.h"
#include "include/Transformations.h"
#include "include/Functions.h"

CSG::CSG(int ID)
{
    Transform = Matrix::Identity();
    TransformInverse = Matrix::Identity();
    Origin = Point(0., 0., 0.);
    AMaterial = Material();
    UseShadow = true;
    this->ID = ID;
    Parent = nullptr;
}

CSG::CSG() : CSG(0)
{
}

CSG::CSG(CSGOp Op, std::shared_ptr<Object> &L, std::shared_ptr<Object> &R) : CSG(0)
{
    Operator = Op;
    Left = L;
    Right = R;
    L->SetParent(this);
    R->SetParent(this);
}

int CSG::GetID()
{
    return ID;
}

std::vector<Intersection<Object>> CSG::LocalIntersect(const Ray &LocalRay)
{
    std::vector<Intersection<Object>> Intersections;

    if (BoundsOf().Intersect(LocalRay)) {
        auto LeftXS = Left->Intersect(LocalRay);
        auto RightXS = Right->Intersect(LocalRay);

        Intersections.insert(Intersections.end(), LeftXS.begin(), LeftXS.end());
        Intersections.insert(Intersections.end(), RightXS.begin(), RightXS.end());
        std::sort(Intersections.begin(), Intersections.end());

        return FilterIntersections(Intersections);
    }

    return Intersections;
}

bool CSG::IntersectionAllowed(bool LHit, bool InL, bool InR)
{
    if (Operator == CSGOp::Union)
    {
        return (LHit && !InR) || (!LHit && !InL);
    }
    else if (Operator == CSGOp::Intersection)
    {
        return (LHit && InR) || (!LHit && InL);
    }
    else if (Operator == CSGOp::Difference)
    {
        return (LHit && !InR) || (!LHit && InL);
    }

    return false;
}

std::vector<Intersection<Object>> CSG::FilterIntersections(std::vector<Intersection<Object>> &XS)
{
    std::vector<Intersection<Object>> Intersections;
    bool LHit = false;
    bool InL = false;
    bool InR = false;

    for (auto &X: XS)
    {
        LHit = Left->Include(X.GetObject());
        if (IntersectionAllowed(LHit, InL, InR))
            Intersections.push_back(X);

        // depending on which object was hit, toggle either InL or InR
        if (LHit)
            InL = !InL;
        else
            InR = !InR;
    }

    return Intersections;
}

bool CSG::Include(Object *S)
{
    if (Left->Include(S) || Right->Include(S))
        return true;
    return false;
}

BoundingBoxes CSG::BoundsOf()
{
    BoundingBoxes Box;
    auto LeftChildBox = Left->ParentSpaceBoundsOf();
    Box.AddBox(LeftChildBox);
    auto RightChildBox = Right->ParentSpaceBoundsOf();
    Box.AddBox(RightChildBox);
    return Box;
}


// TEST_CASE("CSG is created with an operation and two shapes")
// {
//     std::shared_ptr<Object> S1 = std::make_shared<Sphere>(Sphere());
//     std::shared_ptr<Object> S2 = std::make_shared<Cubes>(Cubes());

//     CSG C(CSGOp::Union, S1, S2);
//     Object *CPtr = &C;
//     CHECK(C.GetOp() == CSGOp::Union);
//     CHECK(C.GetLeft() == S1);
//     CHECK(C.GetRight() == S2);
//     CHECK(S1->GetParent() == CPtr);
//     CHECK(S2->GetParent() == CPtr);
// }

// TEST_CASE("Evaluating the rule for a CSG operation")
// {
//     std::shared_ptr<Object> S = std::make_shared<Sphere>(Sphere());
//     auto C = CSG(CSGOp::Union, S, S);
//     CHECK(C.IntersectionAllowed(true, true, true) == false);
//     CHECK(C.IntersectionAllowed(true, true, false) == true);
//     CHECK(C.IntersectionAllowed(true, false, true) == false);
//     CHECK(C.IntersectionAllowed(true, false, false) == true);
//     CHECK(C.IntersectionAllowed(false, true, true) == false);
//     CHECK(C.IntersectionAllowed(false, true, false) == false);
//     CHECK(C.IntersectionAllowed(false, false, true) == true);
//     CHECK(C.IntersectionAllowed(false, false, false) == true);
    
//     C = CSG(CSGOp::Intersection, S, S);
//     CHECK(C.IntersectionAllowed(true, true, true) == true);
//     CHECK(C.IntersectionAllowed(true, true, false) == false);
//     CHECK(C.IntersectionAllowed(true, false, true) == true);
//     CHECK(C.IntersectionAllowed(true, false, false) == false);
//     CHECK(C.IntersectionAllowed(false, true, true) == true);
//     CHECK(C.IntersectionAllowed(false, true, false) == true);
//     CHECK(C.IntersectionAllowed(false, false, true) == false);
//     CHECK(C.IntersectionAllowed(false, false, false) == false);

//     C = CSG(CSGOp::Difference, S, S);
//     CHECK(C.IntersectionAllowed(true, true, true) == false);
//     CHECK(C.IntersectionAllowed(true, true, false) == true);
//     CHECK(C.IntersectionAllowed(true, false, true) == false);
//     CHECK(C.IntersectionAllowed(true, false, false) == true);
//     CHECK(C.IntersectionAllowed(false, true, true) == true);
//     CHECK(C.IntersectionAllowed(false, true, false) == true);
//     CHECK(C.IntersectionAllowed(false, false, true) == false);
//     CHECK(C.IntersectionAllowed(false, false, false) == false);
// }

// TEST_CASE("Filtering a list of intersections")
// {
//     std::shared_ptr<Object> S1 = std::make_shared<Sphere>(Sphere());
//     std::shared_ptr<Object> S2 = std::make_shared<Cubes>(Cubes());

//     std::vector<Intersection<Object>> XS { Intersection<Object>(1., S1.get()),
//                                             Intersection<Object>(2., S2.get()),
//                                             Intersection<Object>(3., S1.get()),
//                                             Intersection<Object>(4., S2.get())};

//     auto C = CSG(CSGOp::Union, S1, S2);
//     auto Result = C.FilterIntersections(XS);
//     CHECK(Result.size() == 2);
//     CHECK(Result[0] == XS[0]);
//     CHECK(Result[1] == XS[3]);

//     C = CSG(CSGOp::Intersection, S1, S2);
//     Result = C.FilterIntersections(XS);
//     CHECK(Result.size() == 2);
//     CHECK(Result[0] == XS[1]);
//     CHECK(Result[1] == XS[2]);

//     C = CSG(CSGOp::Difference, S1, S2);
//     Result = C.FilterIntersections(XS);
//     CHECK(Result.size() == 2);
//     CHECK(Result[0] == XS[0]);
//     CHECK(Result[1] == XS[1]);
// }

// TEST_CASE("A ray misses a CSG object")
// {
//     std::shared_ptr<Object> S1 = std::make_shared<Sphere>(Sphere());
//     std::shared_ptr<Object> S2 = std::make_shared<Cubes>(Cubes());
//     auto C = CSG(CSGOp::Union, S1, S2);
//     Ray R(Point(0., 2., -5.), Vector(0., 0., 1.));
//     auto XS = C.LocalIntersect(R);
//     CHECK(XS.size() == 0);
// }

// TEST_CASE("A ray hits a CSG object")
// {
//     std::shared_ptr<Object> S1 = std::make_shared<Sphere>(Sphere());
//     std::shared_ptr<Object> S2 = std::make_shared<Sphere>(Sphere());
//     S2->SetTransform(Transformations::Translation(0., 0., 0.5));

//     auto C = CSG(CSGOp::Union, S1, S2);
//     Ray R(Point(0., 0., -5.), Vector(0., 0., 1.));
//     auto XS = C.LocalIntersect(R);
//     CHECK(XS.size() == 2);
//     CHECK(XS[0].GetObject() == S1.get());
//     CHECK(Util::Equal(XS[0].GetT(), 4.));
//     CHECK(XS[1].GetObject() == S2.get());
//     CHECK(Util::Equal(XS[1].GetT(), 6.5));
// }
