#ifndef SPHERE_H
#define SPHERE_H

#include "ray.h"
#include "floatvector.h"
#include "material.h"

class Sphere {
    public:
        FloatVec center;
        Material material;
        float radius;

    public:
        Sphere(float x,float y,float z, float radius);
        Sphere(Material mat, float x,float y,float z, float radius);

        float intersect(Ray *ray);
};

#endif //SPHERE_H
