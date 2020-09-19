#pragma once

#include "doctest.h"
#include "Object.h"
#include "Pattern.h"
#include "Point.h"

namespace Fns
{
    Color PatternAtShape(std::shared_ptr<Pattern> &Pat, std::shared_ptr<Object> &Obj, Point &P);
    inline Color PatternAtShape(std::shared_ptr<Pattern> &Pat, std::shared_ptr<Object> &Obj, Point &&P) { return PatternAtShape(Pat, Obj, P); }
    inline Color PatternAtShape(std::shared_ptr<Pattern> &&Pat, std::shared_ptr<Object> &Obj, Point &P) { return PatternAtShape(Pat, Obj, P); }
}