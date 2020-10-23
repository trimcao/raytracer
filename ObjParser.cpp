#include <fstream>
#include <sstream>
#include <string>
#include <exception>
#include <stdexcept>
#include "include/ObjParser.h"
#include "include/Triangles.h"

ObjParser::ObjParser(std::string F, bool Smoothing)
{
    Vertices = std::vector<Point>();
    Normals = std::vector<Vector>();
    IgnoredLines = 0;
    Filename = F;
    TriGroups = std::unordered_map<std::string, std::vector<Triangles>>();
    STriGroups = std::unordered_map<std::string, std::vector<SmoothTriangles>>();
    LatestGroup = "Default";
    TriGroups["Default"] = std::vector<Triangles>();
    STriGroups["Default"] = std::vector<SmoothTriangles>();
    this->Smoothing = Smoothing;
}

void ObjParser::ParseFace(std::vector<int> &VIndices, std::vector<int> &TIndices, std::vector<int> &NIndices,
                    std::string Str)
{
    int Idx;
    std::string Delimiter = "/";
    size_t Pos = 0;
    int ValueType = 0;

    // format of face can be: v/vt/vn
    while ((Pos = Str.find(Delimiter)) != std::string::npos)
    {
        if (Pos != 0)
        {
            std::string Token = Str.substr(0, Pos);
            Idx = std::stoi(Token);
            if (ValueType == 0)
                VIndices.push_back(Idx);
            else if (ValueType == 1)
                TIndices.push_back(Idx);
            else if (ValueType == 2)
                NIndices.push_back(Idx);
            else
                return;
        }
        ++ValueType;
        Str.erase(0, Pos + Delimiter.length());
    }

    std::string Token = Str;
    Idx = std::stoi(Token);
    if (ValueType == 0)
        VIndices.push_back(Idx);
    else if (ValueType == 1)
        TIndices.push_back(Idx);
    else if (ValueType == 2)
        NIndices.push_back(Idx);
    else
        return;
}

void ObjParser::Parse()
{
    std::ifstream InFile(Filename);
    std::string Line;
    while (std::getline(InFile, Line))
    {
        std::istringstream ISS(Line);
        std::string Indicator;
        ISS >> Indicator;

        if (Indicator == "v")
        {
            double Num1, Num2, Num3;
            if (!(ISS >> Num1 >> Num2 >> Num3))
            {
                throw std::invalid_argument("Wrong format for vertex line.");
            }

            Vertices.push_back(Point(Num1, Num2, Num3));
        }
        else if (Indicator == "vn")
        {
            double Num1, Num2, Num3;
            if (!(ISS >> Num1 >> Num2 >> Num3))
            {
                throw std::invalid_argument("Wrong format for vertex normal line.");
            }

            Normals.push_back(Vector(Num1, Num2, Num3));
        }
        else if (Indicator == "f")
        {
            int Idx;
            std::vector<int> VIndices;
            std::vector<int> TIndices;
            std::vector<int> NIndices;
            std::string NextStr;
            while(ISS >> NextStr)
            {
                ParseFace(VIndices, TIndices, NIndices, NextStr);
            }

            // create triangles
            if (NIndices.size() > 0 && Smoothing)
            {
                auto Tris = FanTriangulation(VIndices, TIndices, NIndices);
                STriGroups[LatestGroup].insert(STriGroups[LatestGroup].end(), Tris.begin(), Tris.end());
            }
            else
            {
                auto Tris = FanTriangulation(VIndices);
                TriGroups[LatestGroup].insert(TriGroups[LatestGroup].end(), Tris.begin(), Tris.end());
            }
            
        }
        else if (Indicator == "g")
        {
            std::string GroupName;
            if (!(ISS >> GroupName))
            {
                throw std::invalid_argument("Wrong format for group name.");
            }
            
            if (TriGroups.find(GroupName) == TriGroups.end())
                TriGroups[GroupName] = std::vector<Triangles>();

            if (STriGroups.find(GroupName) == STriGroups.end())
                STriGroups[GroupName] = std::vector<SmoothTriangles>();

            LatestGroup = GroupName;
        }
        else
        {
            ++IgnoredLines;
        }
    }
}

Point ObjParser::GetVertex(int ID)
{
    if (ID < 1 || ID > Vertices.size())
        throw std::invalid_argument("vertex index is invalid");
    
    return Vertices[ID-1];
}

Vector ObjParser::GetNormal(int ID)
{
    if (ID < 1 || ID > Normals.size())
        throw std::invalid_argument("normal index is invalid");

    return Normals[ID-1];
}

std::vector<Triangles> ObjParser::GetGroup(std::string Name)
{
    if (TriGroups.find(Name) == TriGroups.end())
        throw std::invalid_argument("group name is invalid");
    
    return TriGroups[Name];
}

std::vector<SmoothTriangles> ObjParser::GetSGroup(std::string Name)
{
    if (STriGroups.find(Name) == STriGroups.end())
        throw std::invalid_argument("smooth-triangle group name is invalid");

    return STriGroups[Name];
}

std::vector<Triangles> ObjParser::FanTriangulation(std::vector<int>VIndices)
{
    std::vector<Triangles> Tris;

    for (int i = 1; i < (VIndices.size()-1); ++i)
    {
        Triangles Tri(GetVertex(VIndices[0]), GetVertex(VIndices[i]), GetVertex(VIndices[i+1]));
        Tris.push_back(Tri);
    }

    return Tris;
}

std::vector<SmoothTriangles> ObjParser::FanTriangulation(std::vector<int>VIndices, std::vector<int>TIndices, std::vector<int>NIndices)
{
    std::vector<SmoothTriangles> Tris;

    for (int i = 1; i < (VIndices.size()-1); ++i)
    {
        SmoothTriangles Tri(GetVertex(VIndices[0]), GetVertex(VIndices[i]), GetVertex(VIndices[i+1]),
                            GetNormal(NIndices[0]), GetNormal(NIndices[i]), GetNormal(NIndices[i+1]));
        Tris.push_back(Tri);
    }

    return Tris;
}

std::unordered_map<std::string, std::shared_ptr<Groups>> ObjParser::ObjToGroup()
{
    std::unordered_map<std::string, std::shared_ptr<Groups>> OutputGroups;

    for (auto &G: TriGroups)
    {
        auto NewGroup = std::make_shared<Groups>(Groups());
        for (auto &Child: G.second)
        {
            std::shared_ptr<Object> NewChild = std::make_shared<Triangles>(Child);
            NewGroup->AddChild(NewChild);
        }

        OutputGroups[G.first] = NewGroup;
    }

    return OutputGroups;
}

TEST_CASE("Ignoring unrecognized lines")
{
    ObjParser Parser("./test/obj/test1.obj");
    Parser.Parse();
    CHECK(Parser.GetIgnoredLines() == 5);
}

TEST_CASE("Vertex records")
{
    ObjParser Parser("./test/obj/test2.obj");
    Parser.Parse();

    CHECK(Parser.GetVertex(1) == Point(-1., 1. ,0.));
    CHECK(Parser.GetVertex(2) == Point(-1., 0.5 ,0.));
    CHECK(Parser.GetVertex(3) == Point(1., 0. ,0.));
    CHECK(Parser.GetVertex(4) == Point(1., 1. ,0.));
}

TEST_CASE("Parsing triangle faces")
{
    ObjParser Parser("./test/obj/test3.obj");
    Parser.Parse();

    auto G = Parser.GetGroup("Default");
    auto T1 = G[0];
    auto T2 = G[1];

    CHECK(T1.GetP1() == Parser.GetVertex(1));
    CHECK(T1.GetP2() == Parser.GetVertex(2));
    CHECK(T1.GetP3() == Parser.GetVertex(3));

    CHECK(T2.GetP1() == Parser.GetVertex(1));
    CHECK(T2.GetP2() == Parser.GetVertex(3));
    CHECK(T2.GetP3() == Parser.GetVertex(4));
}

TEST_CASE("Triangulating polygons")
{
    ObjParser Parser("./test/obj/test4.obj");
    Parser.Parse();

    auto G = Parser.GetGroup("Default");
    auto T1 = G[0];
    auto T2 = G[1];
    auto T3 = G[2];

    CHECK(T1.GetP1() == Parser.GetVertex(1));
    CHECK(T1.GetP2() == Parser.GetVertex(2));
    CHECK(T1.GetP3() == Parser.GetVertex(3));

    CHECK(T2.GetP1() == Parser.GetVertex(1));
    CHECK(T2.GetP2() == Parser.GetVertex(3));
    CHECK(T2.GetP3() == Parser.GetVertex(4));

    CHECK(T3.GetP1() == Parser.GetVertex(1));
    CHECK(T3.GetP2() == Parser.GetVertex(4));
    CHECK(T3.GetP3() == Parser.GetVertex(5));
}

TEST_CASE("Triangles in groups")
{
    ObjParser Parser("./test/obj/triangles.obj");
    Parser.Parse();

    auto G1 = Parser.GetGroup("FirstGroup");
    auto G2 = Parser.GetGroup("SecondGroup");
    auto T1 = G1[0];
    auto T2 = G2[0];

    CHECK(T1.GetP1() == Parser.GetVertex(1));
    CHECK(T1.GetP2() == Parser.GetVertex(2));
    CHECK(T1.GetP3() == Parser.GetVertex(3));

    CHECK(T2.GetP1() == Parser.GetVertex(1));
    CHECK(T2.GetP2() == Parser.GetVertex(3));
    CHECK(T2.GetP3() == Parser.GetVertex(4));
}

TEST_CASE("Triangles in groups")
{
    ObjParser Parser("./test/obj/triangles.obj");
    Parser.Parse();

    auto G = Parser.ObjToGroup();
    CHECK(G["FirstGroup"]->GetShapes().size() == 1);
    CHECK(G["SecondGroup"]->GetShapes().size() == 1);
}

TEST_CASE("Triangles in groups, with different face format")
{
    ObjParser Parser("./test/obj/triangles2.obj");
    Parser.Parse();

    auto G1 = Parser.GetGroup("Teapot101");
    auto G2 = Parser.GetGroup("Teapot202");
    auto T1 = G1[0];
    auto T2 = G2[0];

    CHECK(T1.GetP1() == Parser.GetVertex(1));
    CHECK(T1.GetP2() == Parser.GetVertex(2));
    CHECK(T1.GetP3() == Parser.GetVertex(3));

    CHECK(T2.GetP1() == Parser.GetVertex(1));
    CHECK(T2.GetP2() == Parser.GetVertex(3));
    CHECK(T2.GetP3() == Parser.GetVertex(4));
}

TEST_CASE("Vertex normal records")
{
    ObjParser Parser("./test/obj/test5.obj");
    Parser.Parse();

    CHECK(Parser.GetNormal(1) == Vector(0., 0. ,1.));
    CHECK(Parser.GetNormal(2) == Vector(0.707, 0. , -0.707));
    CHECK(Parser.GetNormal(3) == Vector(1., 2. ,3.));
}

TEST_CASE("Faces with normals")
{
    ObjParser Parser("./test/obj/test6.obj");
    Parser.Parse();

    auto SG = Parser.GetSGroup("Default");
    auto T1 = SG[0];
    auto T2 = SG[1];

    auto G = Parser.GetGroup("Default");
    auto T3 = G[0];

    CHECK(T1.GetP1() == Parser.GetVertex(1));
    CHECK(T1.GetP2() == Parser.GetVertex(2));
    CHECK(T1.GetP3() == Parser.GetVertex(3));
    CHECK(T1.GetN1() == Parser.GetNormal(3));
    CHECK(T1.GetN2() == Parser.GetNormal(1));
    CHECK(T1.GetN3() == Parser.GetNormal(2));

    CHECK(T2.GetP1() == Parser.GetVertex(1));
    CHECK(T2.GetP2() == Parser.GetVertex(2));
    CHECK(T2.GetP3() == Parser.GetVertex(3));
    CHECK(T2.GetN1() == Parser.GetNormal(3));
    CHECK(T2.GetN2() == Parser.GetNormal(1));
    CHECK(T2.GetN3() == Parser.GetNormal(2));

    CHECK(T3.GetP1() == Parser.GetVertex(1));
    CHECK(T3.GetP2() == Parser.GetVertex(2));
    CHECK(T3.GetP3() == Parser.GetVertex(3));
}

TEST_CASE("Faces with normals, but ignore normals in parser")
{
    ObjParser Parser("./test/obj/test6.obj", false);
    Parser.Parse();

    CHECK(Parser.GetSGroup("Default").size() == 0);
    CHECK(Parser.GetGroup("Default").size() == 3);
}