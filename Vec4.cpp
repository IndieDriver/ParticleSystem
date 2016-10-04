//
// Created by amath on 10/1/2016.
//

#include "Vec4.h"

Vec4	vec_sub(Vec4 v1, Vec4 v2)
{
    Vec4 result;

    result.x = v1.x - v2.x;
    result.y = v1.y - v2.y;
    result.z = v1.z - v2.z;
    return (result);
}

Vec4	vec_add(Vec4 v1, Vec4 v2)
{
    Vec4 result;

    result.x = v1.x + v2.x;
    result.y = v1.y + v2.y;
    result.z = v1.z + v2.z;
    return (result);
}

Vec4	vec_mul(Vec4 v1, Vec4 v2)
{
    Vec4 result;

    result.x = v1.x * v2.x;
    result.y = v1.y * v2.y;
    result.z = v1.z * v2.z;
    return (result);
}

Vec4	vec_cross(Vec4 v1, Vec4 v2)
{
    Vec4 result;

    result.x = v1.y * v2.z - v1.z * v2.y;
    result.y = v1.z * v2.x - v1.x * v2.z;
    result.z = v1.x * v2.y - v1.y * v2.x;
    return (result);
}

Vec4	vec_scale(Vec4 v1, float fact)
{
    Vec4 res;

    res.x = v1.x * fact;
    res.y = v1.y * fact;
    res.z = v1.z * fact;
    return (res);
}


float	dot(Vec4 v1, Vec4 v2)
{
    return (v1.x * v2.x + v1.y * v2.y + v1.z * v2.z);
}

float	dis_point(Vec4 pt1, Vec4 pt2)
{
    return (sqrt((pt2.x - pt1.x) * (pt2.x - pt1.x) + (pt2.y - pt1.y) *
          (pt2.y - pt1.y) + (pt2.z - pt1.z) * (pt2.z - pt1.z)));
}

Vec4	neg_vec(Vec4 vec)
{
    vec.x *= -1;
    vec.y *= -1;
    vec.z *= -1;
    return (vec);
}

float	Vec4::lenght()
{
    return (sqrt(x * x + y * y + z * z));
}

void	Vec4::normalize()
{
    double	fact;
    double	len;

    if ((len = lenght()) == 0)
        len = 0.001;
    fact = 1.0f / len;
    x *= fact;
    y *= fact;
    z *= fact;
}

Vec4::Vec4(float x1, float y1, float z1, float w1) {
    x = x1;
    y = y1;
    z = z1;
    w = w1;
}

void Vec4::print()
{
    printf("%f %f %f %f\n", x, y, z, w);
}
