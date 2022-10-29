#include <stdio.h>
#include <iostream>
#include <string>
#include <sstream>
#include <math.h>
#include "floatvector.h"

using namespace std;

FloatVec FloatVec::operator-(const FloatVec& other)
{
    return FloatVec(x - other.x, y - other.y, z - other.z);
}

FloatVec FloatVec::operator+(const FloatVec& other)
{
    return FloatVec(x + other.x, y + other.y, z + other.z);
}

FloatVec FloatVec::operator*(const FloatVec& other)
{
    return FloatVec(x*other.x, y*other.y, z*other.z);
}

FloatVec FloatVec::operator*(const float scalar)
{
    return FloatVec(x*scalar, y*scalar, z*scalar);
}

FloatVec FloatVec::operator/(const float scalar)
{
    return *this * (1/scalar);
}

FloatVec& FloatVec::operator=(const FloatVec& other)
{
    x = other.x;
    y = other.y;
    z = other.z;

    return *this;
}

FloatVec::FloatVec(): x{1},y{1},z{1} {}

FloatVec::FloatVec(float x, float y, float z): x{x}, y{y}, z{z} {}

FloatVec::~FloatVec() {}

FloatVec FloatVec::normalize()
{
    float mag = this->magnitude();
    //TODO printf("Magnitude: %f\n", mag);

    return FloatVec(x/mag, y/mag, z/mag);
}

float FloatVec::magnitude()
{
    return sqrt((x*x)+(y*y)+(z*z));
}


float FloatVec::dot(FloatVec other)
{
    return x*other.x + y*other.y + z*other.z;
}

FloatVec FloatVec::cross(FloatVec other)
{
    return FloatVec(
            y * other.z - z * other.y,
            z * other.x - x * other.z,
            x * other.y - y * other.x );
}

string FloatVec::toString()
{
    ostringstream output;
    output << "(" << x << ", " << y << ", " << z << ")";

    return output.str();
}

