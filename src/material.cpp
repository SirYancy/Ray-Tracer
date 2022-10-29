#include "material.h"
#include "floatvector.h"

Material::Material():
    ambient{FloatVec(0, 0, 0)},
    diffuse{FloatVec(1, 1, 1)},
    specular{FloatVec(0, 0, 0)},
    ns{5},
    transmissive{FloatVec(0, 0, 0)},
    ior{1} {}


Material::Material(float ar, float ag, float ab,
            float dr, float dg, float db,
            float sr, float sg, float sb,
            float ns,
            float tr, float tg, float tb,
            float ior):
        ambient{FloatVec(ar, ag, ab)},
        diffuse{FloatVec(dr, dg, db)},
        specular{FloatVec(sr, sg, sb)},
        ns{ns},
        transmissive{FloatVec(tr, tg, tb)},
        ior{ior} {}

Material& Material::operator=(const Material& other)
{
    ambient = other.ambient;
    diffuse = other.diffuse;
    specular = other.specular;
    ns = other.ns;
    transmissive = other.transmissive;
    ior = other.ior;

    return *this;
}

