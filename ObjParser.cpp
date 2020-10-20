#include <fstream>
#include <sstream>
#include <string>
#include <exception>
#include <stdexcept>
#include "include/ObjParser.h"
#include "include/Triangles.h"

ObjParser::ObjParser(std::string F)
{
    Vertices = std::vector<Point>();
    IgnoredLines = 0;
    Filename = F;
    TriGroups = std::unordered_map<std::string, std::vector<Triangles>>();
    LatestGroup = "Default";
    TriGroups["Default"] = std::vector<Triangles>();
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
        // std::cout << "Indicator: " << Indicator << '\n';
        if (Indicator == "v")
        {
            double Num1, Num2, Num3;
            if (!(ISS >> Num1 >> Num2 >> Num3))
            {
                throw std::invalid_argument("Wrong format for vertex line.");
            }

            Vertices.push_back(Point(Num1, Num2, Num3));
        }
        else if (Indicator == "f")
        {
            int Idx;
            std::vector<int> Indices;
            // format of face can be: v/vt/vn
            // for now just parse v
            std::string NextStr;
            std::string Delimiter = "/";
            while(ISS >> NextStr)
            {
                std::string Token = NextStr.substr(0, NextStr.find(Delimiter));
                Idx = std::stoi(Token);
                Indices.push_back(Idx);
            }
            
            auto Tris = FanTriangulation(Indices);
            TriGroups[LatestGroup].insert(TriGroups[LatestGroup].end(), Tris.begin(), Tris.end());
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

std::vector<Triangles> ObjParser::GetGroup(std::string Name)
{
    if (TriGroups.find(Name) == TriGroups.end())
        throw std::invalid_argument("group name is invalid");
    
    return TriGroups[Name];
}

std::vector<Triangles> ObjParser::FanTriangulation(std::vector<int>VertexIndices)
{
    std::vector<Triangles> Tris;

    for (int i = 1; i < (VertexIndices.size()-1); ++i)
    {
        Triangles Tri(GetVertex(VertexIndices[0]), GetVertex(VertexIndices[i]), GetVertex(VertexIndices[i+1]));
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