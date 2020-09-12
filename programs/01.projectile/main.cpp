#define DOCTEST_CONFIG_IMPLEMENT
#include <iostream>
#include <vector>
#include <string>
#include "../../include/Matrix.h"
#include "../../include/Point.h"
#include "../../include/Vector.h"
#include "../../include/Util.h"

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
    P.Position = Point(0., 1., 0.f);
    P.Velocity = Vector(1., 1., 0.f).Normalize();

    Environment E;
    E.Gravity = Vector(0., -0.1, 0.f);
    E.Wind = Vector(-0.01, 0., 0.f);

    while (P.Position.Y() >= 0)
    {
        std::cout << "Current Position: " << P.Position << "\n";
        P = Tick(E, P);
    }

    return 0;
}

Projectile Tick(const Environment &Env, const Projectile &Proj)
{
    Projectile NewProj;
    NewProj.Position = Proj.Position + Proj.Velocity;
    NewProj.Velocity = Proj.Velocity + Env.Gravity + Env.Wind;
    return NewProj;
}
