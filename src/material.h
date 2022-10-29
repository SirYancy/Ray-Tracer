#ifndef MATERIAL_H
#define MATERIAL_H

#include "floatvector.h"

class Material {
    public:
        FloatVec ambient;
        FloatVec diffuse;
        FloatVec specular;
        FloatVec transmissive;
        float ns;
        float ior;

    public:
        Material();
        Material(float,float,float,
                 float,float,float,
                 float,float,float,
                 float,float,float,
                 float,float);

    Material& operator=(const Material& other);
};


#endif //MATERIAL_H
