#define DOCTEST_CONFIG_IMPLEMENT
// #define DOCTEST_CONFIG_DISABLE
#include <iostream>
#include <vector>
#include <string>
#include "include/Tuple.h"
#include "include/Point.h"
#include "include/Vector.h"
#include "include/Util.h"
#include "include/main.h"

int main(int argc, char **argv)
{
    // use doctest to run unit tests
    doctest::Context ctx;
    ctx.applyCommandLine(argc, argv);
    int res = ctx.run();
    if (ctx.shouldExit())
        return res;

    // main code
    int res_main = program();

    return res + res_main;
}

int program()
{
    std::vector<std::string> msg{"Hello", "Ray Tracing", "World"};

    for (const std::string &word : msg)
    {
        std::cout << word << " ";
    }
    std::cout << "\n";

    RunProjectile();

    return 0;
}

void RunProjectile()
{
    Projectile P;
    P.Position = Point(0.f, 1.f, 0.f);
    P.Velocity = Vector(1.f, 1.f, 0.f).Normalize();

    Environment E;
    E.Gravity = Vector(0.f, -0.1f, 0.f);
    E.Wind = Vector(-0.01f, 0.f, 0.f);

    while (P.Position.Y >= 0)
    {
        std::cout << "Current Position: " << P.Position << "\n";
        P = Tick(E, P);
    }
}

Projectile Tick(const Environment &Env, const Projectile &Proj)
{
    Projectile NewProj;
    NewProj.Position = Proj.Position + Proj.Velocity;
    NewProj.Velocity = Proj.Velocity + Env.Gravity + Env.Wind;
    return NewProj;
}
