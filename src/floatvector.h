#ifndef FLOATVEC_H
#define FLOATVEC_H

#include <string>

using namespace std;

class FloatVec {

    public:

        float x,y,z;


    public:

        FloatVec();
        FloatVec(float, float, float);
        ~FloatVec();

        float magnitude();

        FloatVec normalize();

        FloatVec& operator=(const FloatVec& other);
        FloatVec  operator+(const FloatVec& other);
        FloatVec  operator-(const FloatVec& other);
        FloatVec  operator*(const float scalar);
        FloatVec  operator*(const FloatVec& other);
        FloatVec  operator/(const float scalar);
        float dot(FloatVec);
        FloatVec cross(FloatVec);

        string toString();
};

#endif //FLOATVEC_H
