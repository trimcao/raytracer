#pragma once

#include "doctest.h"
#include "Object.h"
#include "Pattern.h"
#include "Point.h"

namespace TRay
{
    Color PatternAtShape(std::shared_ptr<Pattern> &Pat, std::shared_ptr<Object> &Obj, Point &P);
    inline Color PatternAtShape(std::shared_ptr<Pattern> &Pat, std::shared_ptr<Object> &Obj, Point &&P) { return PatternAtShape(Pat, Obj, P); }
    inline Color PatternAtShape(std::shared_ptr<Pattern> &&Pat, std::shared_ptr<Object> &Obj, Point &P) { return PatternAtShape(Pat, Obj, P); }

    template<class OT>
    PreComputations<OT> PrepareComputations(Intersection<OT> &I, Ray &R,
        std::vector<Intersection<OT>> IntersectionList = std::vector<Intersection<OT>>{});

    template<class OT>
    std::vector<double> ComputeRefractiveIndex(Intersection<OT> &I,
        std::vector<Intersection<OT>> IntersectionList);

    float Schlick(PreComputations<Object> &Comps);

    Point WorldToObject(Object *O, Point &P);
    inline Point WorldToObject(Object *O, Point &&P) { return WorldToObject(O, P); }

    Vector NormalToWorld(Object *O, Vector &N);
    inline Vector NormalToWorld(Object *O, Vector &&N) { return NormalToWorld(O, N); }
}