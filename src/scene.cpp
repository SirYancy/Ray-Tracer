#include <vector>
#include <math.h>
#include "scene.h"
#include "light.h"
#include "sphere.h"
#include "camera.h"

Scene::Scene():
    width{640}, height{480}, maxDepth{5}
{
    camera = new Camera();
    ambient = new FloatVec(.1, .1, .1);
    background = new FloatVec(0, 0, 0);
}

Scene::~Scene(){ 
    delete camera;
    delete ambient;
    delete background;
    for(auto const& v: lights)
        delete v;
    for(auto const& v: spheres)
        delete v;
}

void Scene::SetMaxDepth(int n)
{
    maxDepth = n;
}

void Scene::SetCamera(Camera *c)
{
    delete camera;
    camera = c;
}

void Scene::SetResolution(int w, int h)
{
    width = w;
    height = h;
}

void Scene::SetAmbient(FloatVec *a)
{
    delete ambient;
    ambient = a;
}

void Scene::SetBackground(FloatVec *b)
{
    delete background;
    background = b;
}

void Scene::AddLight(Light *l)
{
    lights.push_back(l);
}

void Scene::AddSphere(Sphere *s)
{
    spheres.push_back(s);
}

void Scene::Initialize()
{
    float aspect = (float)width / (float)height;

    float hha = camera->ha;
    float fheight = tan(hha)*2;
    float fwidth = fheight * aspect;
    float wha = atan(fwidth/2);

    FloatVec right = camera->up.cross(camera->dir);
    FloatVec rAdjust = right * tan(wha);
    FloatVec uAdjust = camera->up * tan(hha);

    FloatVec p1 = camera->pos + camera->dir - rAdjust
                                       + uAdjust;
    FloatVec p2 = camera->pos + camera->dir + rAdjust
                                       + uAdjust;
    FloatVec p3 = camera->pos + camera->dir + rAdjust
                                       - uAdjust;
    FloatVec p4 = camera->pos + camera->dir - rAdjust
                                       - uAdjust;

    /**
     * p1 ------------ p2
     * |                |
     * |                |
     * |                |
     * p4 ------------ p3
     */

    tl = new FloatVec(p1);
    tr = new FloatVec(p2);
    br = new FloatVec(p3);
    bl = new FloatVec(p4);

}
