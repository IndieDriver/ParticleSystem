//
// Created by amath on 10/1/2016.
//

#ifndef PARTICLESYSTEM_VEC4_H
#define PARTICLESYSTEM_VEC4_H

#include "part.h"

class Vec4{
    public:
        float x;
        float y;
        float z;
        float w;
        Vec4(float x = 0, float y = 0, float z = 0, float w = 0);
        void normalize();
        float lenght();
        void    print();
};

Vec4	vec_sub(Vec4 v1, Vec4 v2);
Vec4	vec_add(Vec4 v1, Vec4 v2);
Vec4	vec_mul(Vec4 v1, Vec4 v2);
Vec4	vec_cross(Vec4 v1, Vec4 v2);
Vec4	vec_scale(Vec4 v1, float fact);
float	dot(Vec4 v1, Vec4 v2);
float	dis_point(Vec4 pt1, Vec4 pt2);
Vec4	neg_vec(Vec4 vec);

#endif //PARTICLESYSTEM_VEC4_H
