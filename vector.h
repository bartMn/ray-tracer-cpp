#ifndef VEC_H
#define VEC_H

#include <cmath>

class vec3 {
public:
    double x, y, z;

    // Constructors
    vec3();
    vec3(double x, double y, double z);

    // Vector operations
    double length() const;
    double length_squared() const;
    void normalize();
    vec3 return_unit();
    static void printVector(const vec3& a);
    // Static utility functions
    static double dot(const vec3& a, const vec3& b);
    static vec3 cross(const vec3& a, const vec3& b);
    static vec3 random(double min, double max);

    // Vector operators
    vec3& operator+=(const vec3& other);
    vec3& operator-=(const vec3& other);
    vec3& operator*=(double scalar);
    vec3& operator/=(double scalar);
};

// Vector operators
vec3 operator+(const vec3& a, const vec3& b);
vec3 operator-(const vec3& a, const vec3& b);
vec3 operator*(const vec3& v, double scalar);
vec3 operator*(const vec3& a, const vec3& b);
vec3 operator*(double scalar, const vec3& v);
vec3 operator/(const vec3& v, double scalar);
vec3 operator-(const vec3& v);


#endif