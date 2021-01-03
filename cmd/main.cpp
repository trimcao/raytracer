#include <iostream>
#include <vector>
#include <string>
#include <cmath>
#include <fstream>
#include <nlohmann/json.hpp>

#include "Point.h"
#include "Vector.h"
#include "Color.h"
#include "Canvas.h"
#include "Sphere.h"
#include "Ray.h"
#include "Light.h"
#include "Intersection.h"
#include "World.h"
#include "Camera.h"
#include "Transformations.h"
#include "Plane.h"
#include "Pattern.h"
#include "Cubes.h"
#include "Cylinders.h"
#include "Triangles.h"
#include "ObjParser.h"


int main(int argc, char **argv)
{
    using nlohmann::json;
    std::cout << "Hello from cmd\n";

    std::ifstream ifs("/Users/trimcao/tri/raytracer/cmd/test.json");
    json jf = json::parse(ifs);

    std::cout << "print: " << jf["json"] << '\n';

    return 0;
}