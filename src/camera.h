/**
 * Camera class
 * Eric Kuha
 */
#ifndef CAMERA_H
#define CAMERA_H

#include "floatvector.h"

class Camera {

    public:
        FloatVec pos;    //Position
        FloatVec dir;    //Viewing direction
        FloatVec up;    //Up vector
        float ha;         //.5 frustum

    public:

        Camera();

        Camera( float px, float py, float pz,
                float dx, float dy, float dz,
                float ux, float uy, float uz,
                float ha);

        ~Camera();

};


#endif //CAMERA_H
