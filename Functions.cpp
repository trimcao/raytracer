#include "include/Functions.h"

Color StripeAtObject(std::shared_ptr<Pattern> &Pat, std::shared_ptr<Object> &Obj, Point &P)
{
    auto LocalPos = Obj->GetTransform().Inverse().Mul(P);
    auto PatternPos = Pat->GetTransform().Inverse().Mul(LocalPos);
    return Pat->StripeAt(PatternPos);
}