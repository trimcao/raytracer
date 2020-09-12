#define DOCTEST_CONFIG_IMPLEMENT
// #define DOCTEST_CONFIG_DISABLE
#include <iostream>
#include <vector>
#include <string>
#include <cmath>
#include <fstream>
#include "../../include/Matrix.h"
#include "../../include/Point.h"
#include "../../include/Vector.h"
#include "../../include/Color.h"
#include "../../include/Util.h"
#include "../../include/Canvas.h"
#include "../../include/Sphere.h"
#include "../../include/Ray.h"
#include "../../include/Intersection.h"

int main(int argc, char **argv)
{
    Canvas CV = Canvas(200, 200);

    // WallZ = Z coordinate of the wall
    double WallZ = 5.;

    // Wall size = the size of the wall relatively to the size of the sphere
    // (the sphere is a unit sphere)
    double WallSize = 5.;
    double PixelSize = WallSize / CV.GetHeight();
    double Half = WallSize / 2.;
    
    Color Red = Color(1., 0., 0.);
    Color Green = Color(0., 1., 0.);

    Point RayOrigin = Point(0., 0., -5.);
    Sphere S(1);

    // add some scaling to the sphere
    S.SetTransform(Matrix::Scaling(0.8, 1., 1.));

    for (int CanvasY = 0; CanvasY < CV.GetHeight(); ++CanvasY)
    {
        for (int CanvasX = 0; CanvasX < CV.GetWidth(); ++CanvasX)
        {
            // we need to flip the Y coordinate because of the convention of
            // Y-coordinate of the canvas
            double WorldY = Half - CanvasY * PixelSize;
            double WorldX = CanvasX * PixelSize - Hal;

            // create a ray
            Point PositionOnWall = Point(WorldX, WorldY, WallZ);
            Ray R(RayOrigin, (PositionOnWall - RayOrigin).Normalize());

            // find the intersection betwee ray and sphere
            auto XS = Intersections(Intersect(R, S));

            // draw the pixel red if there's a hit
            if (Hit(XS) != nullptr)
            {
                CV.WritePixel(CanvasX, CanvasY, Green);
            }
        }        
    }

    std::ofstream out("output.ppm");
    out << CV.ToPPM();
    out.close();

    return 0;
}