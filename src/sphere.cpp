#include <stdio.h>
#include <math.h>
#include <algorithm>
#include "sphere.h"
#include "ray.h"
#include "material.h"

Sphere::Sphere(Material mat, float x,float y,float z, float radius):
    center{FloatVec(x,y,z)}, 
    radius{radius} 
{
    material = mat;
    
}

Sphere::Sphere(float x,float y,float z, float radius):
    center{FloatVec(x,y,z)}, 
    material{Material()},
    radius{radius} {}

float Sphere::intersect(Ray *ray)
{
    float b = ray->dir.dot(ray->origin - center);

    //float descrim = 

    float descrim = b*b - (ray->dir.dot(ray->dir) * 
            ((ray->origin - center).dot(ray->origin - center) - radius*radius));

    if(descrim < 0.0 || isnan(descrim) )
    {
        return -1;
    }

    float t1 = (-ray->dir.dot(ray->origin-center) + sqrt(descrim)) / ray->dir.dot(ray->dir);
    float t2 = (-ray->dir.dot(ray->origin-center) - sqrt(descrim)) / ray->dir.dot(ray->dir);

    return min(t1,t2);
}
