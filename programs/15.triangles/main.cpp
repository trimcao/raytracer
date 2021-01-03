#include <iostream>
#include <vector>
#include <string>
#include <cmath>
#include <fstream>

#include "Point.h"
#include "Vector.h"
#include "Color.h"
#include "Canvas.h"
#include "Sphere.h"
#include "Ray.h"
#include "Light.h"
#include "Intersection.h"
#include "World.h"
#include "Camera.h"
#include "Transformations.h"
#include "Plane.h"
#include "Pattern.h"
#include "Cubes.h"
#include "Cylinders.h"
#include "Triangles.h"
#include "ObjParser.h"

void RoomScene()
{
    // parse the teapot obj
    ObjParser Parser("../teapot-low.obj", true);
    Parser.Parse();

    auto ParsedObjs = Parser.ObjToGroup();

    auto Teapot = ParsedObjs["Teapot001"];

    auto Mat = Material();

    Plane Floor(1);
    Mat = Material();
    Mat.SetColor(Color(0.8, 0.8, 0.9));
    Mat.SetSpecular(0.);
    Mat.SetReflective(0.);
    Mat.SetDiffuse(0.4);
    auto Pat = std::make_shared<CheckersPattern>(Color(192, 197, 204), Color(233, 241, 255));
    Pat->SetTransform(Transformations::RotationY(M_PI/4));
    Mat.SetPattern(Pat);
    Floor.SetMaterial(Mat);

    Plane LeftWall(2);
    LeftWall.SetTransform(Transformations::RotationX(M_PI/2).RotateY(-M_PI/4).Translate(0., 0., 5.));
    Mat = Material();
    Mat.SetColor(Color(227, 208, 191));
    Mat.SetSpecular(0.);
    LeftWall.SetMaterial(Mat);

    Plane RightWall(3);
    RightWall.SetTransform(Transformations::RotationX(M_PI/2).RotateY(M_PI/4).Translate(0., 0., 5.));
    Mat = Material();
    Mat.SetColor(Color(227, 208, 191));
    Mat.SetSpecular(0.);
    RightWall.SetMaterial(Mat);

    Sphere Middle(3);
    Middle.SetTransform(Matrix::Scaling(0.5, 0.5, 0.5).Translate(-1., 0.5, 0.));
    Mat = Material();
    Mat.SetColor(Color(0.1, 1., 0.5));
    Mat.SetDiffuse(0.7);
    Mat.SetSpecular(0.3);
    std::shared_ptr<Pattern> Ring = std::make_shared<StripePattern>(Color(0.5, 1., 0.1), Color(0.98, 0.035, 0.043));
    Ring->SetTransform(Transformations::Scaling(0.2, 1., 1.));
    Mat.SetPattern(Ring);
    Middle.SetMaterial(Mat);

    Sphere Right(4);
    // Right.SetTransform(Matrix::Scaling(0.5, 0.5, 0.5).Translate(0.7, 0.5, 1.));
    Right.SetTransform(Matrix::Scaling(0.33, 0.33, 0.33).Translate(1., 0.33, 0.));
    Mat = Material();
    Mat.SetColor(Color(0.5, 0.1, 1.));
    Mat.SetDiffuse(0.7);
    Mat.SetSpecular(0.3);
    Right.SetMaterial(Mat);
    Right.SetShadowOn(false);

    World W;
    Light L(Color(1., 1., 1.), Point(-10., 8., -10.));
    W.SetLight(L);
    // W.AddObject(TeapotPtr);
    W.AddObject(Floor);
    W.AddObject(LeftWall);
    W.AddObject(RightWall);
    W.AddObject(Middle);
    W.AddObject(Right);

    Camera Cam(640, 360, M_PI/3);
    Cam.SetTransform(Transformations::ViewTransform(Point(0.1, 2., -5.),
                                                    Point(0., 0., 1.),
                                                    Vector(0., 1., 0.)));

    bool RenderShadow = true;

    uint numThreads = 8;
    std::cout << "number of threads used: " << numThreads << '\n';
    auto CV = Cam.Render(W, RenderShadow, true, 5, numThreads);

    std::ofstream out("smooth.ppm");
    out << CV.ToPPM();
    out.close();
}

void TestShadowScene()
{
    auto Mat = Material();

    Plane Floor(1);
    Mat = Material();
    Mat.SetColor(Color(0.8, 0.8, 0.9));
    Mat.SetSpecular(0.);
    Mat.SetReflective(0.);
    Mat.SetDiffuse(0.4);
    auto Pat = std::make_shared<CheckersPattern>(Color(192, 197, 204), Color(233, 241, 255));
    Pat->SetTransform(Transformations::RotationY(M_PI/4));
    Mat.SetPattern(Pat);
    Floor.SetMaterial(Mat);

    Plane LeftWall(2);
    LeftWall.SetTransform(Transformations::RotationX(M_PI/2).RotateY(-M_PI/4).Translate(0., 0., 5.));
    Mat = Material();
    Mat.SetColor(Color(227, 208, 191));
    Mat.SetSpecular(0.);
    LeftWall.SetMaterial(Mat);

    Plane RightWall(3);
    RightWall.SetTransform(Transformations::RotationX(M_PI/2).RotateY(M_PI/4).Translate(0., 0., 5.));
    Mat = Material();
    Mat.SetColor(Color(227, 208, 191));
    Mat.SetSpecular(0.);
    RightWall.SetMaterial(Mat);

    Sphere Middle(3);
    Middle.SetTransform(Matrix::Scaling(0.5, 0.5, 0.5).Translate(-1., 0.5, 0.));
    Mat = Material();
    Mat.SetColor(Color(0.1, 1., 0.5));
    Mat.SetDiffuse(0.7);
    Mat.SetSpecular(0.3);
    std::shared_ptr<Pattern> Ring = std::make_shared<StripePattern>(Color(0.5, 1., 0.1), Color(0.98, 0.035, 0.043));
    Ring->SetTransform(Transformations::Scaling(0.2, 1., 1.));
    Mat.SetPattern(Ring);
    Middle.SetMaterial(Mat);
    // Middle.SetShadowOn(false);

    Sphere Right(4);
    // Right.SetTransform(Matrix::Scaling(0.5, 0.5, 0.5).Translate(0.7, 0.5, 1.));
    Right.SetTransform(Matrix::Scaling(0.33, 0.33, 0.33).Translate(1., 0.33, 0.));
    Mat = Material();
    Mat.SetColor(Color(0.5, 0.1, 1.));
    Mat.SetDiffuse(0.7);
    Mat.SetSpecular(0.3);
    Right.SetMaterial(Mat);
    // Right.SetShadowOn(true);

    World W;
    Light L(Color(1., 1., 1.), Point(-10., 8., -10.));
    W.SetLight(L);
    W.AddObject(Floor);
    W.AddObject(LeftWall);
    W.AddObject(RightWall);
    W.AddObject(Middle);
    W.AddObject(Right);

    Camera Cam(640, 360, M_PI/3);
    Cam.SetTransform(Transformations::ViewTransform(Point(0.1, 2., -5.),
                                                    Point(0., 0., 1.),
                                                    Vector(0., 1., 0.)));

    bool RenderShadow = true;

    uint numThreads = 8;
    std::cout << "number of threads used: " << numThreads << '\n';
    auto CV = Cam.Render(W, RenderShadow, true, 5, numThreads);

    std::ofstream out("shadow.ppm");
    out << CV.ToPPM();
    out.close();
}

int main(int argc, char **argv)
{
    // RoomScene();
    TestShadowScene();

    return 0;
}