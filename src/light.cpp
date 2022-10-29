#include "floatvector.h"
#include "light.h"

Light::Light(float r, float g, float b,
                float px, float py, float pz,
                float dx, float dy, float dz,
                float a1, float a2,
                Type t):
            color{FloatVec(r,g,b)},
            pos{FloatVec(px,py,pz)},
            dir{FloatVec(dx,dy,dz)},
            a1{a1}, a2{a2},
            type{t} {}

