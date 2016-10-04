//
// Created by amath on 10/1/2016.
//

#ifndef PARTICLESYSTEM_MATRIX_H
#define PARTICLESYSTEM_MATRIX_H

#include "part.h"
#include "Vec4.h"

class Matrix {
    public:
        float	mat4[16];
        Matrix();
        void init_matrix();
        void print();
        void get_identity();
        Vec4 mul_matrix4_vec4(const Vec4& vec4);
        void transpose();
};
Matrix get_inverse(Matrix m);
Matrix	mat4_mul(Matrix m1, Matrix m2);
Vec4	mul_matrix4_vec4(Matrix mat, Vec4 vec4);

#endif //PARTICLESYSTEM_MATRIX_H
