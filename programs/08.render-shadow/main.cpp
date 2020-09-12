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
    Floor.SetTransform(Matrix::Scaling(10.f, 0.01f, 10.f));
    auto Mat = Material();
    Mat.SetColor(Color(1.f, 0.9f, 0.9f));
    Mat.SetSpecular(0.f);
    Floor.SetMaterial(Mat);

    Sphere LeftWall(1);
    LeftWall.SetTransform(Matrix::Scaling(10.f, 0.01f, 10.f)
                                .RotateX(M_PI / 2)
                                .RotateY(-M_PI / 4)
                                .Translate(0.f, 0.f, 5.f));
    LeftWall.SetMaterial(Floor.GetMaterial());

    Sphere RightWall(2);
    RightWall.SetTransform(Matrix::Scaling(10.f, 0.01f, 10.f)
                                .RotateX(M_PI / 2)
                                .RotateY(M_PI / 4)
                                .Translate(0.f, 0.f, 5.f));
    RightWall.SetMaterial(Floor.GetMaterial());

    Sphere Middle(3);
    Middle.SetTransform(Matrix::Translation(-0.5f, 1.f, 0.5f));
    Mat = Material();
    Mat.SetColor(Color(0.1f, 1.f, 0.5f));
    Mat.SetDiffuse(0.7f);
    Mat.SetSpecular(0.3f);
    Middle.SetMaterial(Mat);

    Sphere Right(4);
    Right.SetTransform(Matrix::Scaling(0.5f, 0.5f, 0.5f).Translate(1.5f, 0.5f, -0.5f));
    Mat = Material();
    Mat.SetColor(Color(0.5f, 1.f, 0.1f));
    Mat.SetDiffuse(0.7f);
    Mat.SetSpecular(0.3f);
    Right.SetMaterial(Mat);

    Sphere Left(5);
    Left.SetTransform(Matrix::Scaling(0.33f, 0.33f, 0.33f).Translate(-1.5f, 0.33f, -0.75f));
    Mat = Material();
    Mat.SetColor(Color(1.f, 0.8f, 0.1f));
    Mat.SetDiffuse(0.7f);
    Mat.SetSpecular(0.3f);
    Left.SetMaterial(Mat);

    World W;
    Light L(Color(1.f, 1.f, 1.f), Point(-10.f, 10.f, -10.f));
    W.SetLight(L);
    W.AddObject(Floor);
    W.AddObject(LeftWall);
    W.AddObject(RightWall);
    W.AddObject(Middle);
    W.AddObject(Right);
    W.AddObject(Left);

    Camera Cam(100, 50, M_PI/3);
    Cam.SetTransform(Transformations::ViewTransform(Point(0.1f, 1.5f, -5.f),
                                                    Point(0.f, 1.f, 0.f),
                                                    Vector(0.f, 1.f, 0.f)));

    // Rotate the camera in Z direction
    // Cam.SetTransform(Transformations::ViewTransform(Point(0.1f, 1.5f, -5.f),
    //                                                 Point(0.f, 1.f, 0.f),
    //                                                 Vector(0.f, 1.f, 0.f)).RotateZ(M_PI/6));

    bool RenderShadow = true;
    auto CV = Cam.Render(W, RenderShadow, true);

    std::ofstream out("output.ppm");
    out << CV.ToPPM();
    out.close();

    return 0;
}