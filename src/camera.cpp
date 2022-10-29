/**
 * Camera class
 * Eric Kuha
 */
#include <vector>
#include <numeric>
#include <functional>
#include <math.h>

#include "camera.h"
#include "floatvector.h"

Camera::Camera()
{
    pos = FloatVec(0,0,0);

    dir = FloatVec(0,0,1);

    up = FloatVec(0,1,0);

    ha = 45 * M_PI/180;
}

Camera::Camera( float px, float py, float pz,
        float dx, float dy, float dz,
        float ux, float uy, float uz,
        float ha):
    pos{FloatVec(px,py,pz)},
    dir{FloatVec(dx,dy,dz)},
    up{FloatVec(ux,uy,uz)},
    ha{(float)(ha * M_PI/180)} {}

Camera::~Camera(){}
