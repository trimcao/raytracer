#include "include/World.h"
#include "include/Object.h"
#include "include/Sphere.h"
#include "include/Plane.h"
#include "include/Transformations.h"
#include "include/Intersection.h"
#include "include/Functions.h"
#include <cmath>

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
    for (auto &O : Objects)
    {
        auto XS = O->Intersect(R);

        Intersections.insert(Intersections.end(), XS.begin(), XS.end());
    }

    // sort the intersections
    std::sort(Intersections.begin(), Intersections.end());

    return Intersections;
}

Color World::ShadeHit(PreComputations<Object> &Comps, bool RenderShadow, int Remaining)
{
    if (!ALight)
        return Color(0., 0., 0.);

    bool IsInShadow = false;

    if (RenderShadow)
        IsInShadow = IsShadowed(Comps.OverPosition);

    Material Mat;
    if (Comps.AObject->GetParent())
        Mat = Comps.AObject->GetParent()->GetMaterial();
    else
        Mat = Comps.AObject->GetMaterial();

    auto Surface = Lighting(Mat, Comps.AObject, *ALight, Comps.OverPosition, Comps.EyeV, Comps.NormalV, IsInShadow);

    auto Reflected = ReflectedColor(Comps, RenderShadow, Remaining);

    auto Refracted = RefractedColor(Comps, RenderShadow, Remaining);

    if (Mat.GetReflective() > 0. && Mat.GetTransparency() > 0.)
    {
        auto Reflectance = TRay::Schlick(Comps);
        return Surface + Reflected * Reflectance + Refracted * (1 - Reflectance);
    }

    return Surface + Reflected + Refracted;
}

Color World::ColorAt(Ray &R, bool RenderShadow=true, int Remaining=5)
{
    auto Intersects = Intersect(R);
    auto H = Hit(Intersects);
    if (H != nullptr)
    {
        auto Comps = TRay::PrepareComputations(*H, R, Intersects);
        return ShadeHit(Comps, RenderShadow, Remaining);
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
    std::shared_ptr<Intersection<Object>> AHit {nullptr};

    for (auto &I : Intersections)
    {
        if (I.GetT() > 0. && I.GetObject()->ShadowOn())
        {
            AHit = std::make_shared<Intersection<Object>>(I);
            break;
        }
    }

    if (AHit && AHit->GetT() < Distance)
    {
        return true;
    }

    return false;
}

Color World::ReflectedColor(PreComputations<Object> &Comps, bool RenderShadow=true, int Remaining=5)
{
    if (Remaining <= 0)
        return Color(0., 0., 0.);

    auto Reflective = Comps.AObject->GetMaterial().GetReflective();

    if (Util::Equal(Reflective, 0.))
        return Color(0., 0., 0.);

    Ray ReflectedRay(Comps.OverPosition, Comps.ReflectV);

    auto Col = ColorAt(ReflectedRay, RenderShadow, Remaining - 1);
    return Col * Reflective;
}

Color World::RefractedColor(PreComputations<Object> &Comps, bool RenderShadow, int Remaining)
{
    if (Remaining <= 0)
        return Color(0., 0., 0.);

    auto Transparency = Comps.AObject->GetMaterial().GetTransparency();

    // std::cout << "Transparency: " << Transparency << '\n';

    if (Util::Equal(Transparency, 0.))
        return Color(0., 0., 0.);

    // Find the ratio of first index of refraction to the second.
    auto NRatio = Comps.N1 / Comps.N2;
    // cos(theta_i) is the same as the dot product of the two vectors
    auto CosI = Comps.EyeV.Dot(Comps.NormalV);
    // find sin(theta_t)^2 via trigonometric identity
    auto Sin2T = NRatio * NRatio * (1 - CosI * CosI);
    // if Sin2T > 1. we got total internal reflection
    if (Sin2T > 1.)
        return Color(0., 0., 0.);

    // Find cos(theta_t) via trigonometric identity
    auto CosT = std::sqrt(1. - Sin2T);
    auto Direction = Comps.NormalV * (NRatio * CosI - CosT) - Comps.EyeV * NRatio;

    // Create the Refracted Ray
    auto RefractRay = Ray(Comps.UnderPosition, Direction);

    // Find the color of the refracted ray, making sure to multiply
    // by the transparency value to account for any opacity
    return ColorAt(RefractRay, RenderShadow, Remaining - 1) * Transparency;
}

template <class Derived>
void World::AddObject(std::shared_ptr<Derived> &NewObjectPtr)
{
    std::shared_ptr<Object> ObjectPtr = NewObjectPtr;
    AddObject(ObjectPtr);
}

// TEST_CASE("Creating a world")
// {
//     World W;
//     CHECK(W.GetObjects().size() == 0);
//     CHECK(W.GetLight() == nullptr);
// }

// TEST_CASE("The default world")
// {
//     World W = World::DefaultWorld();
//     CHECK(W.GetObjects().size() == 2);
//     CHECK(W.GetLight()->GetIntensity() == Color(1., 1., 1.) );
//     CHECK(W.GetLight()->GetPosition() == Point(-10., 10., -10.) );
// }

// TEST_CASE("Intersect a world with a ray")
// {
//     World W = World::DefaultWorld();
//     Ray R(Point(0., 0., -5.), Vector(0., 0., 1.));

//     auto XS = W.Intersect(R);

//     CHECK(Util::Equal(XS[0].GetT(), 4.));
//     CHECK(Util::Equal(XS[1].GetT(), 4.5));
//     CHECK(Util::Equal(XS[2].GetT(), 5.5));
//     CHECK(Util::Equal(XS[3].GetT(), 6.));

//     // the following value is 4 because two intersections keep a shared_ptr to the object
//     // another owner is World
//     // the last user is the pointer that prints use_count()
//     // std::cout << "Object 1 num user: " << XS[1].GetObject().use_count() << '\n';
// }

// TEST_CASE("Precomputing the state of an intersection")
// {
//     Ray R(Point(0., 0., -5.), Vector(0., 0., 1.));
//     Sphere Shape(1);
//     auto I = Intersection<Sphere>(4., Shape);
//     auto Comps = I.PrepareComputations(R);

//     CHECK(Comps.AObject->GetID() == Shape.GetID());
//     CHECK(Comps.Position == Point(0., 0., -1.));
//     CHECK(Comps.EyeV == Vector(0., 0., -1.));
//     CHECK(Comps.NormalV == Vector(0., 0., -1.));
// }

// TEST_CASE("The hit, when an intersection occurs on the outside")
// {
//     Ray R(Point(0., 0., -5.), Vector(0., 0., 1.));
//     Sphere Shape(1);
//     auto I = Intersection<Object>(4., Shape);
//     auto Comps = I.PrepareComputations(R);

//     CHECK(Comps.IsInside == false);
// }

// TEST_CASE("The hit, when an intersection occurs on the inside")
// {
//     Ray R(Point(0., 0., 0.), Vector(0., 0., 1.));
//     Sphere Shape(1);
//     auto I = Intersection<Sphere>(1., Shape);
//     auto Comps = I.PrepareComputations(R);

//     CHECK(Comps.Position == Point(0., 0., 1.));
//     CHECK(Comps.EyeV == Vector(0., 0., -1.));
//     CHECK(Comps.NormalV == Vector(0., 0., -1.));
//     CHECK(Comps.IsInside == true);
// }

// TEST_CASE("Shading an intersection")
// {
//     auto W = World::DefaultWorld();
//     Ray R(Point(0., 0., -5.), Vector(0., 0., 1.));
//     auto Shape = W.GetObjectAt(0);
//     auto I = Intersection<Object>(4., Shape.get());
//     auto Comps = I.PrepareComputations(R);
//     auto C = W.ShadeHit(Comps);

//     CHECK(C == Color(0.38066, 0.47583, 0.2855));
// }

// TEST_CASE("Shading an intersection from the inside")
// {
//     auto W = World::DefaultWorld();
//     W.SetLight(Light(Color(1., 1., 1.), Point(0., 0.25, 0.)));

//     Ray R(Point(0., 0., 0.), Vector(0., 0., 1.));
//     auto Shape = W.GetObjectAt(1);
//     auto I = Intersection<Object>(0.5, Shape.get());
//     auto Comps = I.PrepareComputations(R);
    
//     bool IsInShadow = W.IsShadowed(Comps.OverPosition);

//     auto C = W.ShadeHit(Comps);

//     CHECK(C == Color(0.90498, 0.90498, 0.90498));
//     CHECK(Comps.IsInside == true);
// }

// TEST_CASE("Smart pointer and polymorphism")
// {
//     std::vector<std::unique_ptr<Object>> X;
//     Sphere S;
//     S.SetTransform(Matrix::Scaling(0.5, 0.5, 0.5));
//     auto V = S.NormalAt(Point(0.5, 0., 0.));

//     // need to use make_unique, otherwise it will complain when free the memory
//     X.push_back(std::make_unique<Sphere>(S));

//     // std::cout << "Pointer location: " << &S << '\n';
//     // std::cout << "Pointer location: " << X[0] << '\n';
//     // std::cout << "Vector location: " << &X << '\n';
//     // std::cout << "Normal vector value: " << V << '\n';
//     // std::cout << "Normal vector value from vector X: " << X[0]->NormalAt(Point(0.5, 0., 0.)) << '\n';
// }

// TEST_CASE("The color when a ray misses")
// {
//     auto W = World::DefaultWorld();
//     Ray R(Point(0., 0., -5.), Vector(0., 1., 0.));

//     auto C = W.ColorAt(R, true);
//     CHECK(C == Color(0., 0., 0.));
// }

// TEST_CASE("The color when a ray hits")
// {
//     auto W = World::DefaultWorld();
//     Ray R(Point(0., 0., -5.), Vector(0., 0., 1.));

//     auto C = W.ColorAt(R, true);
//     CHECK(C == Color(0.38066, 0.47583, 0.2855));
// }

// TEST_CASE("The color with an intersection behind the ray")
// {
//     auto W = World::DefaultWorld();
//     auto Outer = W.GetObjectAt(0);
//     auto Mat = Outer->GetMaterial();
//     Mat.SetAmbient(1.);
//     Outer->SetMaterial(Mat);

//     auto Inner = W.GetObjectAt(1);
//     Mat = Inner->GetMaterial();
//     Mat.SetAmbient(1.);
//     Inner->SetMaterial(Mat);

//     Ray R(Point(0., 0., 0.75), Vector(0., 0., -1.));

//     auto C = W.ColorAt(R, true);
//     CHECK(C == Inner->GetMaterial().GetColor());
// }

// TEST_CASE("There is no shadow when nothing is collinear with point and light")
// {
//     World W = World::DefaultWorld();
//     Point P(0., 10., 0.);
//     CHECK(W.IsShadowed(P) == false);
// }

// TEST_CASE("The shadow when an object is between the point and the light")
// {
//     World W = World::DefaultWorld();
//     Point P(10., -10., 10.);
//     CHECK(W.IsShadowed(P) == true);
// }

// TEST_CASE("There is no shadow when an object is behind the light")
// {
//     World W = World::DefaultWorld();
//     Point P(-20., 20., -20.);
//     CHECK(W.IsShadowed(P) == false);
// }

// TEST_CASE("There is no shadow when an object is behind the point")
// {
//     World W = World::DefaultWorld();
//     Point P(-2., 2., -2.);
//     CHECK(W.IsShadowed(P) == false);
// }

// TEST_CASE("ShadeHit() is given an intersection in shadow")
// {
//     World W;
//     Light L(Color(1., 1., 1.), Point(0., 0., -10.));
//     W.SetLight(L);

//     Sphere S1;
//     W.AddObject(S1);
//     Sphere S2;
//     S2.SetTransform(Matrix::Translation(0., 0., 10.));
//     W.AddObject(S2);

//     Ray R(Point(0., 0., 5.), Vector(0., 0., 1.));
//     Intersection<Object> I(4., S2);
//     auto Comps = I.PrepareComputations(R);

//     auto C = W.ShadeHit(Comps);

//     CHECK(C == Color(0.1, 0.1, 0.1));
// }

// TEST_CASE("The reflected color for a nonreflective material")
// {
//     auto W = World::DefaultWorld();
//     Ray R(Point(0., 0., 0.), Vector(0., 0., 1.));
//     auto S = W.GetObjectAt(1);
//     auto Mat = S->GetMaterial();
//     Mat.SetAmbient(1.);
//     Intersection I(1., S.get());
//     auto Comps = I.PrepareComputations(R);
//     auto C = W.ReflectedColor(Comps);

//     CHECK(C == Color(0., 0., 0.));
// }

// TEST_CASE("The reflected color for a reflective material")
// {
//     auto W = World::DefaultWorld();
//     Ray R(Point(0., 0., -3.), Vector(0., -std::sqrt(2.)/2, std::sqrt(2.)/2));

//     Plane P;
//     Material Mat;
//     Mat.SetReflective(0.5);
//     P.SetMaterial(Mat);
//     P.SetTransform(Transformations::Translation(0., -1., 0.));
//     W.AddObject(P);

//     Intersection I(std::sqrt(2.), W.GetObjectAt(2).get());
//     auto Comps = I.PrepareComputations(R);
//     auto C = W.ReflectedColor(Comps);

//     CHECK(C == Color(0.190332, 0.237915, 0.142749));
// }

// TEST_CASE("ShadeHit with a reflective material")
// {
//     auto W = World::DefaultWorld();
//     Ray R(Point(0., 0., -3.), Vector(0., -std::sqrt(2.)/2, std::sqrt(2.)/2));

//     Plane P;
//     Material Mat;
//     Mat.SetReflective(0.5);
//     P.SetMaterial(Mat);
//     P.SetTransform(Transformations::Translation(0., -1., 0.));
//     W.AddObject(P);

//     Intersection I(std::sqrt(2.), W.GetObjectAt(2).get());
//     auto Comps = I.PrepareComputations(R);
//     auto C = W.ShadeHit(Comps);

//     CHECK(C == Color(0.876757, 0.92434, 0.829174));
// }

// TEST_CASE("ColorAt() with mutually reflective surfaces")
// {
//     World W;
//     W.SetLight(Light(Color(1., 1., 1.), Point(0., 0., 0.)));

//     Material Mat;
//     Mat.SetReflective(1.);
//     Plane Lower;
//     Lower.SetMaterial(Mat);
//     Lower.SetTransform(Transformations::Translation(0., -1., 0.));

//     Plane Higher;
//     Higher.SetMaterial(Mat);
//     Higher.SetTransform(Transformations::Translation(0., 1., 0.));

//     W.AddObject(Lower);
//     W.AddObject(Higher);

//     Ray R(Point(0., 0., 0.), Vector(0., 1., 0.));
//     W.ColorAt(R);
// }

// TEST_CASE("The reflected color at the maximum recursive depth")
// {
//     auto W = World::DefaultWorld();
//     Ray R(Point(0., 0., -3.), Vector(0., -std::sqrt(2.)/2, std::sqrt(2.)/2));

//     Plane P;
//     Material Mat;
//     Mat.SetReflective(0.5);
//     P.SetMaterial(Mat);
//     P.SetTransform(Transformations::Translation(0., -1., 0.));
//     W.AddObject(P);

//     Intersection I(std::sqrt(2.), W.GetObjectAt(2).get());
//     auto Comps = I.PrepareComputations(R);
//     auto C = W.ReflectedColor(Comps, true, 0);

//     CHECK(C == Color(0., 0., 0.));
// }

// TEST_CASE("The refracted color with an opaque surface")
// {
//     auto W = World::DefaultWorld();
//     auto S = W.GetObjectAt(0);
//     Ray R(Point(0., 0., -5.), Vector(0., 0., 1.));
//     std::vector<Intersection<Object>> XS {Intersection(4., S.get()), Intersection(6., S.get())};

//     auto Comps = TRay::PrepareComputations(XS[0], R, XS);
//     auto Col = W.RefractedColor(Comps, true, 5);

//     CHECK(Col == Color(0., 0., 0.));
// }

// TEST_CASE("The refracted color at the maximum recursive depth")
// {
//     auto W = World::DefaultWorld();
//     auto S = W.GetObjectAt(0);

//     auto Mat = S->GetMaterial();
//     Mat.SetTransparency(1.);
//     Mat.SetRefractiveIndex(1.5);
//     S->SetMaterial(Mat);

//     Ray R(Point(0., 0., -5.), Vector(0., 0., 1.));
//     std::vector<Intersection<Object>> XS {Intersection(4., S.get()), Intersection(6., S.get())};

//     auto Comps = TRay::PrepareComputations(XS[0], R, XS);
//     auto Col = W.RefractedColor(Comps, true, 0);

//     CHECK(Col == Color(0., 0., 0.));
// }

// TEST_CASE("The refracted color under total internal reflection")
// {
//     auto W = World::DefaultWorld();
//     auto S = W.GetObjectAt(0);

//     auto Mat = S->GetMaterial();
//     Mat.SetTransparency(1.);
//     Mat.SetRefractiveIndex(1.5);
//     S->SetMaterial(Mat);

//     Ray R(Point(0., 0., std::sqrt(2.)/2), Vector(0., 1., 0.));
//     std::vector<Intersection<Object>> XS   {Intersection(-std::sqrt(2.)/2, S.get()),
//                                             Intersection(std::sqrt(2.)/2, S.get())};

//     auto Comps = TRay::PrepareComputations(XS[1], R, XS);
//     auto Col = W.RefractedColor(Comps, true, 5);

//     CHECK(Col == Color(0., 0., 0.));
// }

// TEST_CASE("The refracted color with a refracted ray")
// {
//     auto W = World::DefaultWorld();
//     auto A = W.GetObjectAt(0);
//     auto B = W.GetObjectAt(1);

//     auto Mat = A->GetMaterial();
//     Mat.SetAmbient(1.);
//     Mat.SetPattern(std::make_shared<TestPattern>());
//     A->SetMaterial(Mat);

//     Mat = B->GetMaterial();
//     Mat.SetTransparency(1.);
//     Mat.SetRefractiveIndex(1.5);
//     B->SetMaterial(Mat);

//     Ray R(Point(0., 0., 0.1), Vector(0., 1., 0.));
//     std::vector<Intersection<Object>> XS   {Intersection(-0.9899, A.get()),
//                                             Intersection(-0.4899, B.get()),
//                                             Intersection(0.4899, B.get()),
//                                             Intersection(0.9899, A.get())};

//     auto Comps = TRay::PrepareComputations(XS[2], R, XS);
//     auto Col = W.RefractedColor(Comps, true, 5);

//     CHECK(Col == Color(0., 0.998875, 0.047219));
// }

// TEST_CASE("ShadeHit() with a transparent material")
// {
//     auto W = World::DefaultWorld();

//     std::shared_ptr<Object> Floor = std::make_shared<Plane>(Plane());
//     Floor->SetTransform(Transformations::Translation(0., -1., 0.));
//     auto Mat = Floor->GetMaterial();
//     Mat.SetTransparency(0.5);
//     Mat.SetRefractiveIndex(1.5);
//     Floor->SetMaterial(Mat);
//     W.AddObject(Floor);

//     std::shared_ptr<Object> Ball = std::make_shared<Sphere>(Sphere());
//     Ball->SetTransform(Transformations::Translation(0., -3.5, -0.5));
//     Mat = Ball->GetMaterial();
//     Mat.SetColor(Color(1., 0., 0.));
//     Mat.SetAmbient(0.5);
//     Ball->SetMaterial(Mat);
//     W.AddObject(Ball);

//     Ray R(Point(0., 0., -3.), Vector(0., -std::sqrt(2.)/2, std::sqrt(2.)/2));
//     std::vector<Intersection<Object>> XS {Intersection(std::sqrt(2.), Floor.get())};

//     auto Comps = TRay::PrepareComputations(XS[0], R, XS);
//     auto Col = W.ShadeHit(Comps, true, 5);

//     CHECK(Col == Color(0.93642, 0.68642, 0.68642));
// }

// TEST_CASE("ShadeHit() with a reflective, transparent material")
// {
//     auto W = World::DefaultWorld();

//     std::shared_ptr<Object> Floor = std::make_shared<Plane>(Plane());
//     Floor->SetTransform(Transformations::Translation(0., -1., 0.));
//     auto Mat = Floor->GetMaterial();
//     Mat.SetTransparency(0.5);
//     Mat.SetReflective(0.5);
//     Mat.SetRefractiveIndex(1.5);
//     Floor->SetMaterial(Mat);
//     W.AddObject(Floor);

//     std::shared_ptr<Object> Ball = std::make_shared<Sphere>(Sphere());
//     Ball->SetTransform(Transformations::Translation(0., -3.5, -0.5));
//     Mat = Ball->GetMaterial();
//     Mat.SetColor(Color(1., 0., 0.));
//     Mat.SetAmbient(0.5);
//     Ball->SetMaterial(Mat);
//     W.AddObject(Ball);

//     Ray R(Point(0., 0., -3.), Vector(0., -std::sqrt(2.)/2, std::sqrt(2.)/2));
//     std::vector<Intersection<Object>> XS {Intersection(std::sqrt(2.), Floor.get())};

//     auto Comps = TRay::PrepareComputations(XS[0], R, XS);
//     auto Col = W.ShadeHit(Comps, true, 5);

//     CHECK(Col == Color(0.93391, 0.69643, 0.69243));
// }