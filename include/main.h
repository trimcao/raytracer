#include "doctest.h"

int program();

// Chapter 1 program
void RunProjectile();

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