#ifndef PARTICLESYSTEM_VEC3_H
#define PARTICLESYSTEM_VEC3_H

#include <math.h>
#include <iostream>

class Vec2;
class Vec3;
class Vec4;

class Vec2 {
public:
    Vec2(float x = 0, float y = 0);
	Vec2(const Vec3 &vec3);
	Vec2(const Vec4 &vec4);
    float   x;
    float   y;
    void    normalize();
    float   length();
    void    print();

    //Vec2    cross(Vec2 v2);
    //Vec2    scale(float fact);
    //float   dot(Vec2 v2);
    Vec2    operator+(const Vec2 &rhs);
    Vec2    operator-(const Vec2 &rhs);
    Vec2    operator*(const Vec2 &rhs);
    Vec2    operator/(const Vec2 &rhs);
	bool    operator==(const Vec2 &rhs) const;
    Vec2    operator+(const float &rhs);
    Vec2    operator-(const float &rhs);
    Vec2    operator*(const float &rhs);
    Vec2    operator/(const float &rhs);
};

class Vec3 {
public:
    Vec3(float x = 0, float y = 0, float z = 0);
	Vec3(const Vec4 &vec4);
	Vec3(const Vec2 &vec2, float z = 0);
    float   x;
    float   y;
    float   z;
    void    normalize();
    float   length();
    void    print();

    Vec3    cross(Vec3 v2);
    Vec3    scale(float fact);
    float   dot(Vec3 v2);
    Vec3    operator+(const Vec3 &rhs);
    Vec3    operator-(const Vec3 &rhs);
    Vec3    operator*(const Vec3 &rhs);
    Vec3    operator/(const Vec3 &rhs);
	Vec3    &operator+=(const Vec3 &rhs);
	Vec3    &operator-=(const Vec3 &rhs);
	bool    operator==(const Vec3 &rhs) const;
	Vec3    operator+(const float &rhs);
    Vec3    operator-(const float &rhs);
    Vec3    operator*(const float &rhs);
    Vec3    operator/(const float &rhs);
};

Vec3	vec_scale(Vec3 v1, float fact);
float	distance(Vec3 pt1, Vec3 pt2);
Vec3	neg_vec(Vec3 vec);

class Vec4{
public:
    float x;
    float y;
    float z;
    float w;
    Vec4(float x = 0, float y = 0, float z = 0, float w = 0);
	Vec4(const Vec2 &vec2, float z = 0, float w = 0);
	Vec4(const Vec3 &vec3, float w = 0);
    void normalize();
    float length();
    void print();
    Vec4 scale(float fact);
    Vec4 cross(Vec4 v2);
    float dot(Vec4 v2);
    Vec4 operator+(const Vec4 &rhs);
    Vec4 operator-(const Vec4 &rhs);
    Vec4 operator*(const Vec4 &rhs);
    Vec4 operator/(const Vec4 &rhs);
    Vec4 operator+(float &f);
    Vec4 operator-(float &f);
    Vec4 operator*(float &f);
    Vec4 operator/(float &f);
};

Vec4	vec_cross(Vec4 v1, Vec4 v2);
Vec4	vec_scale(Vec4 v1, float fact);
float	dot(Vec4 v1, Vec4 v2);
float	distance(Vec4 pt1, Vec4 pt2);
Vec4	neg_vec(Vec4 vec);


#endif //PARTICLESYSTEM_VEC3_H
