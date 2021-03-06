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

struct Projectile
{
    Point Position;
    Vector Velocity;
};

struct Environment
{
    Vector Gravity;
    Vector Wind;
};

Projectile Tick(const Environment &Env, const Projectile &Proj);

int main(int argc, char **argv)
{
    Projectile P;
    P.Position = Point(0., 1., 0.);
    P.Velocity = Vector(1., 1.8, 0.).Normalize() * 11.25;

    Environment E;
    E.Gravity = Vector(0., -0.1, 0.);
    E.Wind = Vector(-0.01, 0., 0.);

    Canvas CV = Canvas(900, 550);
    int Height = CV.GetHeight();

    while (P.Position.Y() >= 0)
    {
        std::cout << "Current Position: " << P.Position << "\n";
        int X = std::round(P.Position.X());
        int Y = std::round(P.Position.Y());
        std::cout << "X, Y: " << X << ' ' << Height - Y << "\n";
        Color Green = Color(0., 1., 0.);

        // draw a square instead of a pixel so it's easier to see
        CV.WritePixel(X, Height - Y, Green);
        CV.WritePixel(X + 1, Height - Y + 1, Green);
        CV.WritePixel(X + 1, Height - Y, Green);
        CV.WritePixel(X, Height - Y + 1, Green);

        P = Tick(E, P);
    }

    std::ofstream out("output.ppm");
    out << CV.ToPPM();
    out.close();

    return 0;
}

Projectile Tick(const Environment &Env, const Projectile &Proj)
{
    Projectile NewProj;
    NewProj.Position = Proj.Position + Proj.Velocity;
    NewProj.Velocity = Proj.Velocity + Env.Gravity + Env.Wind;
    return NewProj;
}
