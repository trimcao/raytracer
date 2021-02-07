#pragma once

#include <set>
#include <filesystem>
#include <unordered_map>
#include "yaml-cpp/yaml.h"
#include "TRay.h"

class Scene
{
    std::filesystem::path scenePath;
    std::filesystem::path outputPath;
    World world;
    Camera cam;
    uint numThreads;
    std::unordered_map<std::string, std::shared_ptr<Object>> definitions;
    const int divideThreshold = 500;

public:
    Scene();

    void Run();

    inline void SetScenePath(char *p)
    {
        scenePath = p;
        std::cout << "Scene: " << std::filesystem::absolute(scenePath) << "\n\n";
    }

    inline void SetOutputPath(char *p)
    {
        outputPath = p;
        if (outputPath.extension() != ".ppm")
        {
            std::cout << "The output file extension " << outputPath.extension() << " will be replaced by \".ppm\"\n";
            outputPath.replace_extension(".ppm");
        }

        std::cout << "Output: " << std::filesystem::absolute(outputPath) << "\n\n";
    }

    inline void SetNumThreads(uint n)
    {
        numThreads = n;
    }

    std::shared_ptr<Object> getObject(const YAML::Node &node, std::string objType);
    Matrix getTransform(const YAML::Node &transforms);

    const static inline std::set<std::string> SHAPES{"sphere", "cube", "plane", "obj", "cylinder", "group"};
};