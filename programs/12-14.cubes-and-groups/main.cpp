#define DOCTEST_CONFIG_IMPLEMENT
// #define DOCTEST_CONFIG_DISABLE
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


void RoomScene()
{
    Plane Floor(1);
    auto Mat = Material();
    Mat.SetColor(Color(0.8, 0.8, 0.9));
    Mat.SetSpecular(0.);
    auto Pat = std::make_shared<CheckersPattern>(Color(0.191, 0.468, 0.449), Color(0., 0.5, 0.5));
    Pat->SetTransform(Transformations::RotationY(M_PI/4));
    Mat.SetPattern(Pat);
    Floor.SetMaterial(Mat);

    Plane LeftWall(2);
    LeftWall.SetTransform(Transformations::RotationX(M_PI/2).RotateY(-M_PI/4).Translate(0., 0., 5.));
    Mat = Material();
    Mat.SetColor(Color(0.8, 0.8, 0.8));
    Mat.SetSpecular(0.);
    LeftWall.SetMaterial(Mat);

    Plane RightWall(3);
    RightWall.SetTransform(Transformations::RotationX(M_PI/2).RotateY(M_PI/4).Translate(0., 0., 5.));
    Mat = Material();
    Mat.SetColor(Color(0.8, 0.8, 0.8));
    Mat.SetSpecular(0.);
    RightWall.SetMaterial(Mat);

    Cubes MirrorFrame(4);
    MirrorFrame.SetTransform(Transformations::Scaling(2.3, 0.01, 1.3).RotateX(M_PI/2).RotateY(M_PI/4).Translate(2., 2., 3.));
    Mat = Material();
    Mat.SetColor(Color(0.265, 0.185, 0.133));
    MirrorFrame.SetMaterial(Mat);

    Cubes Mirror(5);
    Mirror.SetTransform(Transformations::Scaling(2.2, 0.01, 1.2).RotateX(M_PI/2).RotateY(M_PI/4).Translate(2., 2., 3.));
    Mat = Material();
    Mat.SetColor(Color(0.8, 0.8, 0.9));
    Mat.SetDiffuse(0.1);
    Mat.SetSpecular(1.);
    Mat.SetShininess(300.);
    Mat.SetReflective(1.);
    Mirror.SetMaterial(Mat);

    Cubes Poster(4);
    Poster.SetTransform(Transformations::Scaling(0.7, 0.01, 0.7).RotateX(M_PI/2).RotateY(-M_PI/4).Translate(-2.5, 1.25, 2.51));
    Mat = Material();
    Mat.SetColor(Color(1., 0.1, 0.1));
    Poster.SetMaterial(Mat);

    Cubes Poster2(5);
    Poster2.SetTransform(Transformations::Scaling(0.3, 0.01, 0.3).RotateX(M_PI/2).RotateY(-M_PI/4).Translate(-1.4, 1.75, 3.61));
    Mat = Material();
    Mat.SetColor(Color(0.1, 0.1, 1.));
    Poster2.SetMaterial(Mat);

    Cubes Poster3(6);
    Poster3.SetTransform(Transformations::Scaling(0.3, 0.01, 0.3).RotateX(M_PI/2).RotateY(-M_PI/4).Translate(-1.4, 0.75, 3.61));
    Mat = Material();
    Mat.SetColor(Color(0.1, 1., 0.1));
    Poster3.SetMaterial(Mat);

    Cubes TableTop(7);
    TableTop.SetTransform(Transformations::Scaling(1., 0.05, 0.75).RotateY(M_PI/4).Translate(0., 1., -1.));
    Mat = Material();
    Mat.SetColor(Color(0.85, 0.72, 0.53));
    TableTop.SetMaterial(Mat);

    Cubes Leg1(8);
    Leg1.SetTransform(Transformations::Scaling(0.05, 1., 0.05).RotateY(M_PI/4).Translate(-0.2, 0., 0));
    Mat = Material();
    Mat.SetColor(Color(0.85, 0.72, 0.53));
    Leg1.SetMaterial(Mat);

    Cubes Leg2(9);
    Leg2.SetTransform(Transformations::Scaling(0.05, 1., 0.05).RotateY(M_PI/4).Translate(-1., 0., -1.));
    Mat = Material();
    Mat.SetColor(Color(0.85, 0.72, 0.53));
    Leg2.SetMaterial(Mat);

    Cubes Leg3(10);
    Leg3.SetTransform(Transformations::Scaling(0.05, 1., 0.05).RotateY(M_PI/4).Translate(0.15, 0., -1.95));
    Mat = Material();
    Mat.SetColor(Color(0.85, 0.72, 0.53));
    Leg3.SetMaterial(Mat);

    Cubes Leg4(11);
    Leg4.SetTransform(Transformations::Scaling(0.05, 1., 0.05).RotateY(M_PI/4).Translate(1.05, 0., -1.25));
    Mat = Material();
    Mat.SetColor(Color(0.85, 0.72, 0.53));
    Leg4.SetMaterial(Mat);

    World W;
    Light L(Color(1., 1., 1.), Point(-3., 10., -10.));
    W.SetLight(L);
    W.AddObject(Floor);
    W.AddObject(LeftWall);
    W.AddObject(RightWall);
    W.AddObject(Mirror);
    W.AddObject(MirrorFrame);
    W.AddObject(Poster);
    W.AddObject(Poster2);
    W.AddObject(Poster3);
    W.AddObject(TableTop);
    W.AddObject(Leg1);
    W.AddObject(Leg2);
    W.AddObject(Leg3);
    W.AddObject(Leg4);

    Camera Cam(192, 108, M_PI/3);
    Cam.SetTransform(Transformations::ViewTransform(Point(0.1, 2., -5.),
                                                    Point(0., 0., 1.),
                                                    Vector(0., 1., 0.)));

    bool RenderShadow = true;
    auto CV = Cam.Render(W, RenderShadow, true, 5);

    std::ofstream out("output.ppm");
    out << CV.ToPPM();
    out.close();
}

int main(int argc, char **argv)
{
    RoomScene();

    return 0;
}