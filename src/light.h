#ifndef LIGHT_H
#define LIGHT_H

#include "floatvector.h"

enum Type {SPOT, DIRECTIONAL, POINT};

class Light
{
    public:
        FloatVec color;
        FloatVec pos;
        FloatVec dir;
        float a1,a2;
        Type type;

    public:
        Light(float r, float g, float b,
                float px, float py, float pz,
                float dx, float dy, float dz,
                float a1, float a2,
                Type t);
};

#endif //LIGHT_H
