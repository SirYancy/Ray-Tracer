#ifndef HIT_H
#define HIT_H

#include "sphere.h"
#include "floatvector.h"

class Sphere;
class Hit{
    public:
        Sphere *sphere;
        FloatVec pos;
        FloatVec normal;

    public:
        Hit();
        Hit(Sphere *s, FloatVec p, FloatVec n);
};

#endif //HIT_H
