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

int main(int argc, char **argv)
{
    Plane Floor(0);
    // Floor.SetTransform(Matrix::Scaling(10., 0.01, 10.));
    auto Mat = Material();
    Mat.SetColor(Color(1., 0.9, 0.9));
    Mat.SetSpecular(0.);
    Mat.SetReflective(0.75);
    Mat.SetPattern(std::make_shared<CheckersPattern>(Color(0.75, 0.25, 0.), Color(1., 0.9, 0.9)));
    Floor.SetMaterial(Mat);

    // Sphere FrontWall(1);
    // FrontWall.SetTransform(Matrix::Scaling(10., 0.01, 10.)
    //                             .RotateX(M_PI / 2)
    //                             .RotateY(-M_PI / 4)
    //                             .Translate(0., 0., 5.));
    // FrontWall.SetMaterial(Floor.GetMaterial());

    // Sphere RightWall(2);
    // RightWall.SetTransform(Matrix::Scaling(10., 0.01, 10.)
    //                             .RotateX(M_PI / 2)
    //                             .RotateY(M_PI / 4)
    //                             .Translate(0., 0., 5.));
    // RightWall.SetMaterial(Floor.GetMaterial());

    Sphere Middle(3);
    Middle.SetTransform(Matrix::Scaling(0.5, 0.5, 0.5).Translate(-0.5, 0.5, 4.));
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
    Right.SetTransform(Matrix::Scaling(0.33, 0.33, 0.33).Translate(0.2, 0.33, 3.));
    Mat = Material();
    Mat.SetColor(Color(0.5, 0.1, 1.));
    Mat.SetDiffuse(0.7);
    Mat.SetSpecular(0.3);
    Right.SetMaterial(Mat);

    auto Front = Sphere::GlassSphere();
    Front.SetTransform(Matrix::Scaling(1., 1., 1.).Translate(0., 1., -1.5));
    Mat = Front.GetMaterial();
    Mat.SetColor(Color(0.8, 0.8, 0.9));
    Mat.SetAmbient(0.);
    Mat.SetDiffuse(0.2);
    Mat.SetSpecular(0.9);
    Mat.SetShininess(300.);
    Mat.SetTransparency(0.8);
    Mat.SetRefractiveIndex(1.57);
    Front.SetMaterial(Mat);

    World W;
    Light L(Color(1., 1., 1.), Point(-10., 10., -10.));
    W.SetLight(L);
    W.AddObject(Floor);
    // W.AddObject(FrontWall);
    // W.AddObject(RightWall);
    W.AddObject(Middle);
    W.AddObject(Right);
    W.AddObject(Front);

    Camera Cam(640, 480, M_PI/3);
    Cam.SetTransform(Transformations::ViewTransform(Point(0.1, 1.5, -5.),
                                                    Point(0., 1., 0.),
                                                    Vector(0., 1., 0.)));

    // Rotate the camera in Z direction
    // Cam.SetTransform(Transformations::ViewTransform(Point(0.1, 1.5, -5.),
    //                                                 Point(0., 1., 0.),
    //                                                 Vector(0., 1., 0.)).RotateZ(M_PI/6));

    bool RenderShadow = true;
    auto CV = Cam.Render(W, RenderShadow, true, 5);

    std::ofstream out("refraction2.ppm");
    out << CV.ToPPM();
    out.close();

    return 0;
}