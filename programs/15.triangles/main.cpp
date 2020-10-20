#define DOCTEST_CONFIG_IMPLEMENT
// #define DOCTEST_CONFIG_DISABLE
#include <iostream>
#include <vector>
#include <string>
#include <cmath>
#include <fstream>

#include "../../include/doctest.h"
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
    ObjParser Parser("/Users/trimcao/tri/raytracer/programs/15.triangles/teapot-low.obj");
    Parser.Parse();

    auto ParsedObjs = Parser.ObjToGroup();

    for (auto &G: ParsedObjs)
    {
        std::cout << "Name: " << G.first << '\n';
    }

    auto Teapot = ParsedObjs["Teapot001"];

    // std::shared_ptr<Object> TeapotPtr = std::make_shared<Groups>(Teapot);

    auto TeapotShapes = Teapot->GetShapes();

    // std::cout << "Parent before: " <<  TeapotShapes[0]->GetParent() << '\n';
    // TeapotShapes[0]->SetParent(nullptr);
    // std::cout << "Parent after: " <<  TeapotShapes[0]->GetParent() << '\n';

    // W.AddObject(TeapotShapes[0]);
    // W.AddObject(TeapotShapes[1]);
    auto Mat = Material();
    Mat.SetColor(Color(0., 0., 0.5));

    for (auto &O: TeapotShapes)
    {
        // O->SetParent(nullptr);
        // O->SetTransform(Transformations::Scaling(0.1, 0.1, 0.1));
        // W.AddObject(O);
        O->SetMaterial(Mat);
    }

    Plane Floor(1);
    Mat = Material();
    Mat.SetColor(Color(0.8, 0.8, 0.9));
    Mat.SetSpecular(0.);
    auto Pat = std::make_shared<CheckersPattern>(Color(0.414, 0.293, 0.207), Color(0.332, 0.234, 0.164));
    Pat->SetTransform(Transformations::RotationY(M_PI/4));
    Mat.SetPattern(Pat);
    Floor.SetMaterial(Mat);

    Plane LeftWall(2);
    LeftWall.SetTransform(Transformations::RotationX(M_PI/2).RotateY(-M_PI/4).Translate(0., 0., 5.));
    Mat = Material();
    Mat.SetColor(Color(0.969, 0.902, 0.816));
    Mat.SetSpecular(0.);
    LeftWall.SetMaterial(Mat);

    Plane RightWall(3);
    RightWall.SetTransform(Transformations::RotationX(M_PI/2).RotateY(M_PI/4).Translate(0., 0., 5.));
    Mat = Material();
    Mat.SetColor(Color(0.969, 0.902, 0.816));
    Mat.SetSpecular(0.);
    RightWall.SetMaterial(Mat);

    Groups TestGroup(4);
    std::shared_ptr<Object> FloorPtr = std::make_shared<Plane>(Floor);
    std::shared_ptr<Object> LeftWallPtr = std::make_shared<Plane>(LeftWall);
    std::shared_ptr<Object> RightWallPtr = std::make_shared<Plane>(RightWall);
    TestGroup.AddChild(FloorPtr);
    TestGroup.AddChild(LeftWallPtr);
    TestGroup.AddChild(RightWallPtr);

    World W;
    Light L(Color(1., 1., 1.), Point(-3., 10., -10.));
    W.SetLight(L);
    // W.AddObject(TestGroup);
    std::shared_ptr<Object> TeapotPtr = Teapot;
    TeapotPtr->SetTransform(Transformations::Scaling(0.1, 0.1, 0.1).RotateX(-M_PI/2));
    W.AddObject(TeapotPtr);

    std::cout << "Number of Objects: " << W.GetObjects().size() << '\n';

    Camera Cam(64, 36, M_PI/3);
    Cam.SetTransform(Transformations::ViewTransform(Point(0.1, 2., -5.),
                                                    Point(0., 0., 1.),
                                                    Vector(0., 1., 0.)));

    bool RenderShadow = true;

    // auto CV = Cam.Render(W, RenderShadow, true, 5);

    // std::ofstream out("output.ppm");
    // out << CV.ToPPM();
    // out.close();
}

int main(int argc, char **argv)
{
    RoomScene();

    return 0;
}