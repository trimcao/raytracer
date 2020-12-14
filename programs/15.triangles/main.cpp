#include <iostream>
#include <vector>
#include <string>
#include <cmath>
#include <fstream>

#include "../../include/Point.h"
#include "../../include/Vector.h"
#include "../../include/Color.h"
#include "../../include/Canvas.h"
#include "../../include/Sphere.h"
#include "../../include/Ray.h"
#include "../../include/Light.h"
#include "../../include/Intersection.h"
#include "../../include/World.h"
#include "../../include/Camera.h"
#include "../../include/Transformations.h"
#include "../../include/Plane.h"
#include "../../include/Pattern.h"
#include "../../include/Cubes.h"
#include "../../include/Cylinders.h"
#include "../../include/Triangles.h"
#include "../../include/ObjParser.h"


void RoomScene()
{
    // parse the teapot obj
    ObjParser Parser("../teapot-low.obj", true);
    Parser.Parse();

    auto ParsedObjs = Parser.ObjToGroup();

    auto Teapot = ParsedObjs["Teapot001"];

    auto Mat = Material();
    Mat.SetColor(Color(255, 151, 52));
    Mat.SetDiffuse(0.7);
    Mat.SetSpecular(0.3);

    std::shared_ptr<Object> TeapotPtr = Teapot;
    TeapotPtr->SetTransform(Transformations::Scaling(0.1, 0.1, 0.1).RotateX(-M_PI/2).Translate(0., 0., -1.));
    TeapotPtr->SetMaterial(Mat);

    Plane Floor(1);
    Mat = Material();
    Mat.SetColor(Color(0.8, 0.8, 0.9));
    Mat.SetSpecular(0.);
    Mat.SetReflective(0.5);
    Mat.SetDiffuse(0.2);
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

    World W;
    Light L(Color(1., 1., 1.), Point(-10., 8., -10.));
    W.SetLight(L);
    W.AddObject(TeapotPtr);
    W.AddObject(Floor);
    W.AddObject(LeftWall);
    W.AddObject(RightWall);

    Camera Cam(192, 108, M_PI/3);
    Cam.SetTransform(Transformations::ViewTransform(Point(0.1, 2., -5.),
                                                    Point(0., 0., 1.),
                                                    Vector(0., 1., 0.)));

    bool RenderShadow = true;

    auto CV = Cam.Render(W, RenderShadow, true, 5, true);

    std::ofstream out("smooth.ppm");
    out << CV.ToPPM();
    out.close();
}

int main(int argc, char **argv)
{
    RoomScene();

    return 0;
}