
#include <iostream>
#include <vector>
#include <string>
#include <cmath>
#include <fstream>

#include "yaml-cpp/yaml.h"
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

// TODO: group does not use child's material?

class Scene
{
    World *world;
    Camera *camera;

public:
};

int main(int argc, char **argv)
{

    YAML::Node scene = YAML::LoadFile("/Users/trimcao/tri/raytracer/scenes/table.yml");

    // setup objects for the scene
    World world;
    Camera cam;
    Groups mainGroup;

    for (YAML::const_iterator it = scene.begin(); it != scene.end(); ++it)
    {
        const YAML::Node &node = *it;
        if (node["add"])
        {
            std::string objType = node["add"].as<std::string>();

            if (objType == "camera")
            {
                cam = Camera(node["width"].as<int>(), node["height"].as<int>(), node["field_of_view"].as<double>());

                Point from = Point(node["from"][0].as<double>(),
                                   node["from"][1].as<double>(),
                                   node["from"][2].as<double>());
                Point to = Point(node["to"][0].as<double>(),
                                 node["to"][1].as<double>(),
                                 node["to"][2].as<double>());
                Vector up = Vector(node["up"][0].as<double>(),
                                   node["up"][1].as<double>(),
                                   node["up"][2].as<double>());
                cam.SetTransform(Transformations::ViewTransform(from, to, up));
            }
            else if (objType == "cube")
            {
                auto cube = std::make_shared<Cubes>();
                // set transform matrix
                auto transforms = node["transform"];
                Matrix transformMatrix = Matrix::Identity();
                for (YAML::const_iterator ite = transforms.begin(); ite != transforms.end(); ++ite)
                {
                    const YAML::Node &transformNode = *ite;
                    auto transformType = transformNode[0].as<std::string>();
                    if (transformType == "translate")
                    {
                        auto x = transformNode[1].as<double>();
                        auto y = transformNode[2].as<double>();
                        auto z = transformNode[3].as<double>();
                        transformMatrix = transformMatrix.Translate(x, y, z);
                    }
                    else if (transformType == "scale")
                    {
                        auto x = transformNode[1].as<double>();
                        auto y = transformNode[2].as<double>();
                        auto z = transformNode[3].as<double>();
                        transformMatrix = transformMatrix.Scale(x, y, z);
                    }
                    else if (transformType == "rotate-x")
                    {
                        transformMatrix = transformMatrix.RotateX(transformNode[1].as<double>());
                    }
                    else if (transformType == "rotate-y")
                    {
                        transformMatrix = transformMatrix.RotateY(transformNode[1].as<double>());
                    }
                    else if (transformType == "rotate-z")
                    {
                        transformMatrix = transformMatrix.RotateZ(transformNode[1].as<double>());
                    }
                }

                cube->SetTransform(transformMatrix);

                // set shadow
                if (node["shadow"])
                {
                    cube->SetShadowOn(node["shadow"].as<bool>());
                }

                // set material
                if (node["material"])
                {
                    auto materialNode = node["material"];
                    Material material;

                    // TODO: add patterned material

                    if (materialNode["ambient"])
                    {
                        material.SetAmbient(materialNode["ambient"].as<double>());
                    }
                    if (materialNode["diffuse"])
                    {
                        material.SetDiffuse(materialNode["diffuse"].as<double>());
                    }
                    if (materialNode["specular"])
                    {
                        material.SetSpecular(materialNode["specular"].as<double>());
                    }
                    if (materialNode["shininess"])
                    {
                        material.SetShininess(materialNode["shininess"].as<double>());
                    }
                    if (materialNode["reflective"])
                    {
                        material.SetReflective(materialNode["reflective"].as<double>());
                    }
                    if (materialNode["color"])
                    {
                        material.SetColor(Color(materialNode["color"][0].as<double>(),
                                                materialNode["color"][1].as<double>(),
                                                materialNode["color"][2].as<double>()));
                    }
                    if (materialNode["transparency"])
                    {
                        material.SetTransparency(materialNode["transparency"].as<double>());
                    }
                    if (materialNode["refractive_index"])
                    {
                        material.SetRefractiveIndex(materialNode["refractive_index"].as<double>());
                    }

                    cube->SetMaterial(material);
                }

                std::shared_ptr<Object> cubePtr = cube;
                // mainGroup.AddChild(cubePtr);
                world.AddObject(cubePtr);
            }
            else if (objType == "light")
            {
                Color intensity = Color(node["intensity"][0].as<double>(),
                                        node["intensity"][1].as<double>(),
                                        node["intensity"][2].as<double>());
                Point at = Point(node["at"][0].as<double>(),
                                 node["at"][1].as<double>(),
                                 node["at"][2].as<double>());
                Light light(intensity, at);
                world.SetLight(light);
            }
        }
        else if (node["define"])
        {
            std::cout << "Define section: " << node["define"].as<std::string>() << '\n';
        }
    }

    // divide the mainGroup
    // mainGroup.Divide(2);
    world.AddObject(mainGroup);

    // render
    bool renderShadow = true;

    uint numThreads = 8;
    std::cout << "number of threads used: " << numThreads << '\n';
    auto canvas = cam.Render(world, renderShadow, true, 5, numThreads);

    std::ofstream out("output.ppm");
    out << canvas.ToPPM();
    out.close();

    return 0;
}
