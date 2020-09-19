#pragma once

#include "doctest.h"
#include "Object.h"
#include "Pattern.h"
#include "Point.h"

Color StripeAtObject(std::shared_ptr<Pattern> &Pat, std::shared_ptr<Object> &Obj, Point &P);
inline Color StripeAtObject(std::shared_ptr<Pattern> &Pat, std::shared_ptr<Object> &Obj, Point &&P) { return StripeAtObject(Pat, Obj, P); }