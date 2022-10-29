
#include "floatvector.h"
#include "sphere.h"
#include "hit.h"

Hit::Hit() {}

Hit::Hit(Sphere *s, FloatVec p, FloatVec n): 
    sphere{s}, pos{p}, normal{n} {}
