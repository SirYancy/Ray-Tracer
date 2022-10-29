#ifndef SCENE_H
#define SCENE_H

#include <vector>
#include "light.h"
#include "sphere.h"
#include "camera.h"
#include "floatvector.h"

class Sphere;
class Scene {
    public:

        Camera *camera;
        int width, height;
        int maxDepth;
        FloatVec *tl, *tr, *br, *bl;
        FloatVec *ambient;
        FloatVec *background;
        vector<Light*> lights;
        vector<Sphere*> spheres;

    public:

        Scene();
        ~Scene();

        void SetCamera(Camera *c);
        void SetResolution(int w, int h);
        void SetAmbient(FloatVec *a);
        void SetBackground(FloatVec *b);
        void SetMaxDepth(int n);
        void AddLight(Light *l);
        void AddSphere(Sphere *s);
        void Initialize();


};

#endif //SCENE_H

