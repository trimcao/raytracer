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
    std::vector<Vector> Normals;
    int IgnoredLines;
    std::string Filename;
    std::unordered_map<std::string, std::vector<Triangles>> TriGroups;
    std::unordered_map<std::string, std::vector<SmoothTriangles>> STriGroups;
    std::string LatestGroup;

    std::vector<Triangles> FanTriangulation(std::vector<int>VertexIndices);

    std::vector<SmoothTriangles> FanTriangulation(std::vector<int>VertexIndices,
                                            std::vector<int>TextureIndices,
                                            std::vector<int>NormalIndices);

    void ParseFace(std::vector<int> &VIndices, std::vector<int> &TIndices, std::vector<int> &NIndices,
                    std::string Str);

public:
    ObjParser(std::string F);

    Point GetVertex(int ID);
    Vector GetNormal(int ID);
    std::vector<Triangles> GetGroup(std::string Name);
    std::vector<SmoothTriangles> GetSGroup(std::string Name);
    inline std::vector<Point> GetVertices() { return Vertices; }
    inline std::vector<Vector> GetNormals() { return Normals; }
    inline int GetIgnoredLines() { return IgnoredLines; }

    std::unordered_map<std::string, std::shared_ptr<Groups>> ObjToGroup();

    void Parse();
};