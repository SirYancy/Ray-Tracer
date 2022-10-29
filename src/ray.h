#ifndef RAY_H
#define RAY_H

#include <string>
#include "scene.h"
#include "hit.h"

using namespace std;

class Scene;
class Ray{

    public:
        FloatVec origin; //point
        FloatVec dir; //direction
        Hit *hit;
        float t;
        float tMin, tMax;
        int level;

    public:
        Ray(FloatVec point, FloatVec dir);
        Ray(Scene *scene, int i, int j);
        Ray(Ray *ray, FloatVec normal);
        ~Ray();

        bool FindIntersection(Scene *scene);

        string toString();
};

#endif //RAY_H
