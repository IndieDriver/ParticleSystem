#ifndef PARTICLESYSTEM_MATRIX_H
#define PARTICLESYSTEM_MATRIX_H

#include "Vec3.h"
//TODO: namespace

class Matrix {
    public:
        float	mat4[16];
        Matrix();
        void init_matrix();
        void print();
        void get_identity();
		Vec4 operator*(const Vec4 &rhs);
		Matrix operator*(const Matrix &rhs);
        void transpose();
};
Matrix  inverse(const Matrix &rhs);

Matrix  getMVP(Matrix model, Matrix view, Matrix proj);
Matrix  modelMatrix(Vec3 position = (0.0f, 0.0f, 0.0f), Vec3 rot = Vec3(0.0f, 0.0f, 0.0f), Vec3 scale = Vec3(1.0f, 1.0f, 1.0f));
Matrix  viewMatrix(Vec3 pos, Vec3 center, Vec3 up);
Matrix  projMatrix(float fov, float ratio);
Matrix  orthoMatrix(float left, float right, float top, float bottom, float n, float f);
void    printMatrix(float array[16]);


#endif //PARTICLESYSTEM_MATRIX_H
