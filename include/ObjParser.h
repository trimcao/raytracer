#pragma once

#include <unordered_map>
#include <vector>
#include "doctest.h"
#include "Object.h"
#include "Pattern.h"
#include "Point.h"
#include "Groups.h"
#include "Triangles.h"

class ObjParser
{
    std::vector<Point> Vertices;
    int IgnoredLines;
    std::string Filename;
    std::unordered_map<std::string, std::vector<Triangles>> TriGroups;
    std::string LatestGroup;

    std::vector<Triangles> FanTriangulation(std::vector<int>VertexIndices);

public:
    ObjParser(std::string F);

    Point GetVertex(int ID);
    std::vector<Triangles> GetGroup(std::string Name);
    inline std::vector<Point> GetVertices() { return Vertices; }
    inline int GetIgnoredLines() { return IgnoredLines; }

    std::unordered_map<std::string, std::shared_ptr<Groups>> ObjToGroup();

    void Parse();
};