#include <algorithm>
#include <cassert>
#include <cstdlib>
#include <cstring>
#include <cstdio>
#include <iostream>
#include <sstream>
#include <fstream>
#include <string>

#include "scene.h"
#include "ray.h"
#include "hit.h"
#include "camera.h"
#include "image.h"
#include "pixel.h"
#include "floatvector.h"
#include "sphere.h"
#include "material.h"
#include "light.h"

#define STB_IMAGE_IMPLEMENTATION //only place once in one .cpp file
#include "stb_image.h"

#define STB_IMAGE_WRITE_IMPLEMENTATION //only place once in one .cpp files
#include "stb_image_write.h"

using namespace std;

int FloatClamp (float i)
{ return (i<0) ? 0 : (i>255) ? 255 : (int)i; }

FloatVec ClampVec(FloatVec i);

// TODO Replace with something more robust
Scene *scene;

float Distance(FloatVec a, FloatVec b);

Image *RayTrace();
static void ShowUsage(void);
void ParseFile(string fn);
void Output(Image *img, string fn);
FloatVec Ambient(Hit *hit);
FloatVec ApplyLightingModel(Ray *ray);
FloatVec DiffuseContribution(Light *l, Hit *h);
FloatVec SpecularContribution(Light *l, Ray *r, Hit *h);
Ray Reflect(Ray *ray, FloatVec normal);
FloatVec EvaluateRayTree(Scene *scene, Ray mirror);

int main(int argc, char* argv[]){
    // first argument is program name
    argv++, argc--;

    if (argc == 0){
        ShowUsage();
    }

    scene = new Scene();

    char* filename = argv[0];

    ParseFile(filename);

    Image *img = RayTrace();

    Output(img, filename);

    delete scene;
}

void ParseFile(string fn)
{
    FILE *fp;
    long length;
    char line[1024];

    Material *mat = new Material();

    fp = fopen(fn.c_str(), "r");

    if (fp == NULL)
    {
        printf("Can't open the file %s\n", fn.c_str());
    }

    fseek(fp, 0, SEEK_END);
    length = ftell(fp);
    printf("File is '%s' is %ld bytes long \n\n", fn.c_str(), length);
    fseek(fp, 0, SEEK_SET);

    while( fgets(line, 1024, fp))
    {
        if(line[0] == '#'){
            continue;
        }

        char command[100];
        int fieldsRead = sscanf(line, "%s", command);

        if(fieldsRead < 1)
        {
            // Blank line
            continue;
        }
        if(!strcmp(command, "camera")){
            float px, py, pz,
                  dx, dy, dz,
                  ux, uy, uz,
                  ha;

            sscanf(line, "camera %f %f %f %f %f %f %f %f %f %f",
                  &px, &py, &pz,
                  &dx, &dy, &dz,
                  &ux, &uy, &uz,
                  &ha);

            Camera *camera = new Camera(px,py,pz, dx,dy,dz, ux,uy,uz, ha);
            scene->SetCamera(camera);

            printf("A Camera:\n%.2f %.2f %.2f\n%.2f %.2f %.2f\n%.2f %.2f %.2f\n%.2f\n",
                  px, py, pz,
                  dx, dy, dz,
                  ux, uy, uz,
                  ha);
        }
        else if(!strcmp(command, "film_resolution")){
            int w,h;
            sscanf(line, "film_resolution %d %d", &w, &h);
            scene->SetResolution(w,h);
            printf("Output will be: (%d, %d)\n", w, h);
        }
        else if(!strcmp(command, "sphere")){
            float x,y,z,r;
            sscanf(line, "sphere %f %f %f %f", &x, &y, &z, &r);
            scene->AddSphere(new Sphere(*mat, x, y, z, r));
            printf("Sphere as position (%.2f, %.2f, %.2f) with radius %.2f\n", x,y,z,r);
        }
        else if(!strcmp(command, "ambient_light")){
            float r,g,b;
            sscanf(line, "ambient_light %f %f %f", &r, &g, &b);
            scene->SetAmbient(new FloatVec(r,g,b));
            printf("Ambient light of color (%.2f,%.2f,%.2f)\n", r,g,b);
        }
        else if(!strcmp(command, "point_light")){
            float r,g,b;
            float x,y,z;
            sscanf(line, "point_light %f %f %f %f %f %f", &r, &g, &b, &x, &y, &z);
            scene->AddLight(new Light(r,g,b,x,y,z,0,0,0,0,0,POINT));
            printf("Point light of color (%.2f,%.2f,%.2f)\n", r,g,b);
        }
        else if(!strcmp(command, "directional_light")){
            float r,g,b;
            float x,y,z;
            sscanf(line, "directional_light %f %f %f %f %f %f", &r, &g, &b, &x, &y, &z);
            scene->AddLight(new Light(r,g,b,0,0,0,x,y,z,0,0,DIRECTIONAL));
            printf("Directional light of color (%.2f,%.2f,%.2f)\n", r,g,b);
        }
        else if(!strcmp(command, "spot_light")){
            float r,g,b;
            float px,py,pz;
            float dx,dy,dz;
            float a1,a2;

            sscanf(line, "spot_light %f %f %f %f %f %f %f %f %f %f %f", &r, &g, &b, 
                                                                        &px, &py, &pz,
                                                                        &dx, &dy, &dz,
                                                                        &a1, &a2);
            scene->AddLight(new Light(r,g,b,px,py,pz,dx,dy,dz,a1,a2,SPOT));
            printf("Spot light of color (%.2f,%.2f,%.2f)\n", r,g,b);
        }
        else if(!strcmp(command, "material")){
            float ar, ag, ab,
                  dr, dg, db,
                  sr, sg, sb, ns,
                  tr, tg, tb, ior;

            sscanf(line, "material %f %f %f %f %f %f %f %f %f %f %f %f %f %f",
                    &ar, &ag, &ab,
                    &dr, &dg, &db,
                    &sr, &sg, &sb, &ns,
                    &tr, &tg, &tb, &ior);

            // Set the current material to this material.
            delete mat;
            mat = new Material( ar, ag, ab,
                                dr, dg, db,
                                sr, sg, sb, ns,
                                tr, tg, tb, ior);

            printf("A Material with the following properties:\n%.2f %.2f %.2f\n%.2f %.2f %.2f\n%.2f %.2f %.2f %.2f\n%.2f %.2f %.2f %.2f\n",
                      ar, ag, ab,
                      dr, dg, db,
                      sr, sg, sb, ns,
                      tr, tg, tb, ior);
        }
        else if(!strcmp(command, "background")){
            float r,g,b;
            sscanf(line, "background %f %f %f", &r, &g, &b);
            scene->SetBackground(new FloatVec(r,g,b));
            printf("Background color of (%.2f,%.2f,%.2f)\n", r,g,b);
        }
        else if(!strcmp(command, "output_image")){
            char outFile[1024];
            sscanf(line, "output_image %s", outFile);
            printf("Render to file named: %s\n", outFile);
        }
        else if(!strcmp(command, "max_depth")){
            int n;
            sscanf(line, "max_depth %d", &n);
            scene->SetMaxDepth(n);
            printf("Max Depth: %d\n", n);
        }
        else {
            printf("WARNING. Unknown command: %s\n", command);
        }
    }
    scene->Initialize();
}

Image *RayTrace() 
{
    FloatVec *background = scene->background;
    Camera *camera = scene->camera;
    int width = scene->width;
    int height = scene->height;

    Image *img = new Image(width, height);

    Pixel white(255,255,255);
    Pixel back(background->x*255, background->y*255, background->z*255);

    for(int i = 0; i < width; i++){
        for(int j = 0; j < height; j++){
            Ray ray(scene, i, j);
            bool hitfound = ray.FindIntersection(scene);
            if(hitfound)
            {
                FloatVec color = ApplyLightingModel(&ray) * 255 ;
                color = ClampVec(color);
                img->SetPixel(i, j, color);
            }
            else
                img->SetPixel(i,j,back);
        }
    }

    return img;
}

void Output(Image *img, string fn)
{
    string suffix = ".png";

    string delim = ".";

    string prefix = fn.substr(0, fn.find(delim));

    string outFile = prefix + suffix;

    img->Write(outFile.c_str());

}

FloatVec black(0,0,0);

FloatVec ApplyLightingModel(Ray *ray)
{
    Hit *hit = ray->hit;
    FloatVec contribution = black;
    for(auto const& light : scene->lights)
    {
        Ray shadow(hit->pos, light->pos - hit->pos);
        bool blocked = shadow.FindIntersection(scene);
        if(blocked && shadow.t < Distance(light->pos, hit->pos))
        {
            continue;
        }
        contribution = contribution + DiffuseContribution(light, hit);
        contribution = contribution + SpecularContribution(light, ray, hit);
    }
    // Don't reflect forever!
    if(ray->level < scene->maxDepth)
    {
        Ray mirror = Reflect(ray, hit->normal);
        contribution = contribution + hit->sphere->material.specular * EvaluateRayTree(scene, mirror);
    }

    contribution = contribution + Ambient(hit);
    return contribution;
}

FloatVec DiffuseContribution(Light *light, Hit *hit)
{
    Sphere *s = hit->sphere;

    FloatVec l = light->pos - hit->pos;
    l = l.normalize();

    float d = Distance(light->pos, hit->pos);

    float r = (s->material.diffuse.x * light->color.x * max((float)0, hit->normal.dot(l)))/(d*d);
    float g = (s->material.diffuse.y * light->color.y * max((float)0, hit->normal.dot(l)))/(d*d);
    float b = s->material.diffuse.z * light->color.z * max((float)0, hit->normal.dot(l))/(d*d);

    return FloatVec(r,g,b);
}

FloatVec SpecularContribution(Light *light, Ray *ray, Hit *hit)
{
    Sphere *s = hit->sphere;

    FloatVec l = light->pos - hit->pos;
    l = l.normalize();

    FloatVec v = ray->origin - hit->pos;
    v = v.normalize();

    FloatVec h = v + l;
    h = h.normalize();

    float d = Distance(light->pos, hit->pos);

    float r = (s->material.specular.x * light->color.x * pow(max((float)0, hit->normal.dot(h)), s->material.ns))/(d*d);
    float g = (s->material.specular.y * light->color.y * pow(max((float)0, hit->normal.dot(h)), s->material.ns))/(d*d);
    float b = (s->material.specular.z * light->color.z * pow(max((float)0, hit->normal.dot(h)), s->material.ns))/(d*d);

    return FloatVec(r,g,b);
}

Ray Reflect(Ray *ray, FloatVec normal)
{
    return Ray(ray, normal);
}

FloatVec EvaluateRayTree(Scene *scene, Ray mirror)
{
    FloatVec color;
    bool hitfound = mirror.FindIntersection(scene);
    if(hitfound)
    {
        color = ApplyLightingModel(&mirror);
        color = ClampVec(color);
        return color;
    }
    else
    {
        return *(scene->background);
    }
}

FloatVec Ambient(Hit *hit)
{
    FloatVec ambient = *(scene->ambient);
    FloatVec matambient = hit->sphere->material.ambient;
    return ambient * matambient;
}

static void ShowUsage(void)
{
    fprintf(stderr, "Usage: ray <filename>\n");
    exit(EXIT_FAILURE);
}

float Distance(FloatVec a, FloatVec b)
{
    return sqrt(pow(a.x - b.x, 2) + pow(a.y - b.y, 2) + pow(a.z - b.z, 2));
}

FloatVec ClampVec(FloatVec i)
{
    float r = (i.x<0) ? 0 : (i.x>255) ? 255 : i.x;
    float g = (i.y<0) ? 0 : (i.y>255) ? 255 : i.y;
    float b = (i.z<0) ? 0 : (i.z>255) ? 255 : i.z;

    return FloatVec(r,g,b);
}
