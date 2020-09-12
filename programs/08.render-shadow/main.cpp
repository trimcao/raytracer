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

int main(int argc, char **argv)
{
    Sphere Floor(0);
    Floor.SetTransform(Matrix::Scaling(10., 0.01, 10.f));
    auto Mat = Material();
    Mat.SetColor(Color(1., 0.9, 0.9f));
    Mat.SetSpecular(0.f);
    Floor.SetMaterial(Mat);

    Sphere LeftWall(1);
    LeftWall.SetTransform(Matrix::Scaling(10., 0.01, 10.f)
                                .RotateX(M_PI / 2)
                                .RotateY(-M_PI / 4)
                                .Translate(0., 0., 5.f));
    LeftWall.SetMaterial(Floor.GetMaterial());

    Sphere RightWall(2);
    RightWall.SetTransform(Matrix::Scaling(10., 0.01, 10.f)
                                .RotateX(M_PI / 2)
                                .RotateY(M_PI / 4)
                                .Translate(0., 0., 5.f));
    RightWall.SetMaterial(Floor.GetMaterial());

    Sphere Middle(3);
    Middle.SetTransform(Matrix::Translation(-0.5, 1., 0.5f));
    Mat = Material();
    Mat.SetColor(Color(0.1, 1., 0.5f));
    Mat.SetDiffuse(0.7f);
    Mat.SetSpecular(0.3f);
    Middle.SetMaterial(Mat);

    Sphere Right(4);
    Right.SetTransform(Matrix::Scaling(0.5, 0.5, 0.5f).Translate(1.5, 0.5, -0.5f));
    Mat = Material();
    Mat.SetColor(Color(0.5, 1., 0.1f));
    Mat.SetDiffuse(0.7f);
    Mat.SetSpecular(0.3f);
    Right.SetMaterial(Mat);

    Sphere Left(5);
    Left.SetTransform(Matrix::Scaling(0.33, 0.33, 0.33f).Translate(-1.5, 0.33, -0.75f));
    Mat = Material();
    Mat.SetColor(Color(1., 0.8, 0.1f));
    Mat.SetDiffuse(0.7f);
    Mat.SetSpecular(0.3f);
    Left.SetMaterial(Mat);

    World W;
    Light L(Color(1., 1., 1.f), Point(-10., 10., -10.f));
    W.SetLight(L);
    W.AddObject(Floor);
    W.AddObject(LeftWall);
    W.AddObject(RightWall);
    W.AddObject(Middle);
    W.AddObject(Right);
    W.AddObject(Left);

    Camera Cam(800, 400, M_PI/3);
    Cam.SetTransform(Transformations::ViewTransform(Point(0.1, 1.5, -5.f),
                                                    Point(0., 1., 0.f),
                                                    Vector(0., 1., 0.f)));

    // Rotate the camera in Z direction
    // Cam.SetTransform(Transformations::ViewTransform(Point(0.1, 1.5, -5.f),
    //                                                 Point(0., 1., 0.f),
    //                                                 Vector(0., 1., 0.f)).RotateZ(M_PI/6));

    bool RenderShadow = true;
    auto CV = Cam.Render(W, RenderShadow, true);

    std::ofstream out("output.ppm");
    out << CV.ToPPM();
    out.close();

    return 0;
}