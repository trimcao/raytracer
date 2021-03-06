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

void Draw(Canvas &CV, double X, double Y, Color &C);

int main(int argc, char **argv)
{
    Canvas CV = Canvas(900, 550);
    int MidHeight = CV.GetHeight() / 2.;
    int MidWidth = CV.GetWidth() / 2.;

    Color Green = Color(0., 1., 0.);

    double Scaling = 200.;

    Point P = Point(0., 1., 0.);
    double X = P.X() * Scaling;
    double Y = P.Y() * Scaling;

    // draw a square instead of a pixel so it's easier to see
    Draw(CV, MidWidth + X, CV.GetHeight() - (MidHeight + Y), Green);

    for (int i = 0; i < 12; ++i)
    {
        P = P.RotateZ(M_PI / 6);
        std::cout << P << '\n';
        X = P.X() * Scaling;
        Y = P.Y() * Scaling;

        Draw(CV, MidWidth + X, CV.GetHeight() - (MidHeight + Y), Green);
    }

    std::ofstream out("output.ppm");
    out << CV.ToPPM();
    out.close();

    return 0;
}

void Draw(Canvas &CV, double X, double Y, Color &C)
{
    CV.WritePixel(X, Y, C);
    CV.WritePixel(X + 1, Y + 1, C);
    CV.WritePixel(X + 1, Y, C);
    CV.WritePixel(X, Y + 1, C);
}