#include <iostream>
#include <math.h>
#include <sstream>
#include <string>
#include <vector>
#include "floatvector.h"
#include "camera.h"
#include "scene.h"
#include "hit.h"
#include "sphere.h"

using namespace std;

Ray::Ray(FloatVec p, FloatVec d)
{
    origin = p;
    dir = d.normalize();
}

/**
 * For finding rays from the camera directly. Takes the scene object as input.
 */
Ray::Ray(Scene *scene, int i, int j) : tMin{0}, tMax{100}
{

    int width = scene->width;
    int height = scene->height;

    Camera *camera = scene->camera;

    origin = camera->pos;


    FloatVec p1 = *(scene->tl);
    FloatVec p2 = *(scene->tr);
    FloatVec p3 = *(scene->br);
    FloatVec p4 = *(scene->bl);

    // Bilinear interpolation
    FloatVec px1 = p1 + (p2 - p1) * (i + .5) / (float)width;
    FloatVec px2 = p4 + (p3 - p4) * (i + .5) / (float)width;

    FloatVec p = px1 + (px2 - px1) * (j + .5) / (float)height;

    dir = (p - origin);

    dir = dir.normalize();

    level = 0;
}

/**
 * For finding reflection rays. Computes the ray from the original ray
 * and the surface normal
 */
Ray::Ray(Ray *ray, FloatVec normal) : tMin{0}, tMax{100}
{
    origin = (ray->hit->pos);
    dir = ray->dir - normal * (ray->dir.dot(normal) * 2);
    dir = dir.normalize();
    level = ray->level + 1;

    /*
    printf("In Ray: %s\n", ray->dir.toString().c_str());

    printf("origin: %s\ndir: %s\nnormal: %s\n\n",
            origin.toString().c_str(),
            dir.toString().c_str(),
            normal.toString().c_str());
            */
}

Ray::~Ray(){}

bool Ray::FindIntersection(Scene *scene)
{
    bool hitfound = false;
    t = -1;
    Sphere *hitSphere;
    int i = 0;
    for(auto const& sphere: scene->spheres)
    {
        i++;
        float test = sphere->intersect(this);
        if (test > 0)
        {
            if(hitfound)
            {
                if(test < t)
                {
                    t = test;
                    hitSphere = sphere;
                }
            }
            else
            {
                t = test;
                hitSphere = sphere;
                hitfound = true;
            }
        }
    }

    if(t < 0 || isnan(t))
    {
        return false;
    }

    FloatVec p = origin + dir * t;
    FloatVec n = (p - hitSphere->center).normalize();

    hit = new Hit(hitSphere, FloatVec(p), FloatVec(n));
    return true;
}

string Ray::toString()
{
    ostringstream output;
    output << endl
        << "Ray:" << endl
        << "Point:" << origin.x << " " << origin.y << " " << origin.z << endl
        << "Direction:" << dir.x << " " << dir.y << " " << dir.z << endl;

    return output.str();
}


