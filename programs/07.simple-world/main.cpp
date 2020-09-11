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
    // Canvas CV = Canvas(200, 200);

    // // WallZ = Z coordinate of the wall
    // float WallZ = 5.f;

    // // Wall size = the size of the wall relatively to the size of the sphere
    // // (the sphere is a unit sphere)
    // float WallSize = 5.f;
    // float PixelSize = WallSize / CV.GetHeight();
    // float Half = WallSize / 2.f;

    // Color Red = Color(1.f, 0.f, 0.f);
    // Color Green = Color(0.f, 1.f, 0.f);

    // Point RayOrigin = Point(0.f, 0.f, -5.f);
    // Sphere S;
    // Material M;
    // M.SetColor(Color(1.f, 0.2f, 1.f));
    // S.SetMaterial(M);

    // // add some scaling to the sphere
    // S.SetTransform(Matrix::Scaling(0.7f, 1.f, 1.f));

    // // add a light source
    // Light L(Color(1.f, 1.f, 1.f), Point(-10.f, 10.f, -10.f));

    // // variables for progress bar
    // int TotalPixels = CV.GetHeight() * CV.GetWidth();
    // int CurPixel = 0;
    // int Step = 1;
    // int DisplayNext = 1;

    // for (int CanvasY = 0; CanvasY < CV.GetHeight(); ++CanvasY)
    // {
    //     for (int CanvasX = 0; CanvasX < CV.GetWidth(); ++CanvasX)
    //     {
    //         // std::cout << "X: " << CanvasX << ", Y: " << CanvasY << "\n";

    //         // we need to flip the Y coordinate because of the convention of
    //         // Y-coordinate of the canvas
    //         float WorldY = Half - CanvasY * PixelSize;
    //         float WorldX = CanvasX * PixelSize - Half;

    //         // create a ray
    //         Point PositionOnWall = Point(WorldX, WorldY, WallZ);
    //         Ray R(RayOrigin, (PositionOnWall - RayOrigin).Normalize());

    //         // find the intersection betwee ray and sphere
    //         auto XS = Intersections(Intersect(R, S));

    //         // draw the pixel red if there's a hit
    //         auto CurrentHit = Hit(XS);
    //         if (CurrentHit != nullptr)
    //         {
    //             auto HitPoint = R.Position(CurrentHit->GetT());

    //             auto CurrentSphere = CurrentHit->GetObject();

    //             if (CurrentSphere != nullptr)
    //             {
    //                 // std::cout << "Sphere ID: " << CurrentSphere->GetID() << '\n';
    //                 // std::cout << "Pointer use count: " << CurrentSphere.use_count() << '\n';
    //                 // std::cout << "Sphere's transform matrix: " << CurrentSphere->GetTransform() << '\n';

    //                 auto NormalV = S.NormalAt(HitPoint);
    //                 auto EyeV = -R.GetDirection();
    //                 // std::cout << "Normal Vector: " << NormalV << '\n';
    //                 // std::cout << "Eye Vector: " << EyeV << '\n';

    //                 auto ColorToDraw = Lighting(M, L, HitPoint, EyeV, NormalV);
    //                 // std::cout << "Color To Draw: " << ColorToDraw << '\n';
    //                 CV.WritePixel(CanvasX, CanvasY, ColorToDraw);
    //             }
    //         }

    //         // Formatted progress indicator
    //         ++CurPixel;
    //         auto Percent = (100 * (CurPixel + 1)) / TotalPixels ;
    //         if (Percent >= DisplayNext)
    //         {
    //             std::cout << "\r" << "Progress [" << std::string(Percent / 5, '=') << std::string(100 / 5 - Percent / 5, ' ') << "]";
    //             std::cout << ' ' << Percent << "%";
    //             std::cout.flush();
    //             DisplayNext += Step;
    //         }
    //     }
    // }

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

    Camera Cam(800, 400, M_PI/3);
    Cam.SetTransform(Transformations::ViewTransform(Point(0.1f, 1.5f, -5.f),
                                                    Point(0.f, 1.f, 0.f),
                                                    Vector(0.f, 1.f, 0.f)));

    auto CV = Cam.Render(W);

    std::ofstream out("output.ppm");
    out << CV.ToPPM();
    out.close();

    return 0;
}