//
// Created by amath on 10/1/2016.
//

#include "Matrix.h"

Matrix::Matrix() {
    init_matrix();
}

void Matrix::print()
{
    int i;

    i = 0;
    while (i < 16)
    {
        printf("%f ", mat4[i]);
        if ((i + 1) % 4 == 0)
            printf("\n");
        i++;
    }
    printf("\n");
}

void Matrix::init_matrix()
{
    int i;

    i = 0;
    while (i < 16)
    {
        mat4[i] = 0;
        i++;
    }
}

void Matrix::get_identity()
{
    init_matrix();
    mat4[0] = 1.0f;
    mat4[5] = 1.0f;
    mat4[10] = 1.0f;
    mat4[15] = 1.0f;
}

Vec4	Matrix::mul_matrix4_vec4(const Vec4& vec4)
{
    Vec4 out(0.0f);

    out.x = (mat4[0] * vec4.x) + (mat4[1] * vec4.y)
            + (mat4[2] * vec4.z) + (mat4[3] * vec4.w);
    out.y = (mat4[4] * vec4.x) + (mat4[5] * vec4.y)
            + (mat4[6] * vec4.z) + (mat4[7] * vec4.w);
    out.z = (mat4[8] * vec4.x) + (mat4[9] * vec4.y)
            + (mat4[10] * vec4.z) + (mat4[11] * vec4.w);
    out.w = (mat4[12] * vec4.x) + (mat4[13] * vec4.y)
            + (mat4[14] * vec4.z) + (mat4[15] * vec4.w);
    return (out);
}

void	Matrix::transpose() {
    float tmp;
    int i;
    int j;

    i = 0;
    while (i < 3) {
        j = i + 1;
        while (j < 4) {
            tmp = mat4[i * 4 + j];
            mat4[i * 4 + j] = mat4[j * 4 + i];
            mat4[j * 4 + i] = tmp;
            j++;
        }
        i++;
    }
}

Matrix	mat4_mul(Matrix m1, Matrix m2)
{
    Matrix res;
    float sum;
    int i;
    int	j;
    int k;

    i = 0;
    while (i < 4)
    {
        j = 0;
        while (j < 4)
        {
            k = 0;
            sum = 0.0f;
            while (k < 4)
            {
                sum += (m1.mat4[i * 4 + k] * m2.mat4[k * 4 + j]);
                k++;
            }
            res.mat4[i * 4 + j] = sum;
            j++;
        }
        i++;
    }
    return (res);
}

Vec4	mul_matrix4_vec4(Matrix mat, Vec4 vec4)
{
    Vec4 out;

    out.x = (mat.mat4[0] * vec4.x) + (mat.mat4[1] * vec4.y)
            + (mat.mat4[2] * vec4.z) + (mat.mat4[3] * vec4.w);
    out.y = (mat.mat4[4] * vec4.x) + (mat.mat4[5] * vec4.y)
            + (mat.mat4[6] * vec4.z) + (mat.mat4[7] * vec4.w);
    out.z = (mat.mat4[8] * vec4.x) + (mat.mat4[9] * vec4.y)
            + (mat.mat4[10] * vec4.z) + (mat.mat4[11] * vec4.w);
    out.w = (mat.mat4[12] * vec4.x) + (mat.mat4[13] * vec4.y)
            + (mat.mat4[14] * vec4.z) + (mat.mat4[15] * vec4.w);
    return (out);
}

Matrix get_inverse(Matrix m)
{
    Matrix out;
    Matrix inv;
    float det;
    int i;

    inv.mat4[0] = m.mat4[5]  * m.mat4[10] * m.mat4[15] -
             m.mat4[5]  * m.mat4[11] * m.mat4[14] -
             m.mat4[9]  * m.mat4[6]  * m.mat4[15] +
             m.mat4[9]  * m.mat4[7]  * m.mat4[14] +
             m.mat4[13] * m.mat4[6]  * m.mat4[11] -
             m.mat4[13] * m.mat4[7]  * m.mat4[10];

    inv.mat4[4] = -m.mat4[4]  * m.mat4[10] * m.mat4[15] +
             m.mat4[4]  * m.mat4[11] * m.mat4[14] +
             m.mat4[8]  * m.mat4[6]  * m.mat4[15] -
             m.mat4[8]  * m.mat4[7]  * m.mat4[14] -
             m.mat4[12] * m.mat4[6]  * m.mat4[11] +
             m.mat4[12] * m.mat4[7]  * m.mat4[10];

    inv.mat4[8] = m.mat4[4]  * m.mat4[9] * m.mat4[15] -
             m.mat4[4]  * m.mat4[11] * m.mat4[13] -
             m.mat4[8]  * m.mat4[5] * m.mat4[15] +
             m.mat4[8]  * m.mat4[7] * m.mat4[13] +
             m.mat4[12] * m.mat4[5] * m.mat4[11] -
             m.mat4[12] * m.mat4[7] * m.mat4[9];

    inv.mat4[12] = -m.mat4[4]  * m.mat4[9] * m.mat4[14] +
              m.mat4[4]  * m.mat4[10] * m.mat4[13] +
              m.mat4[8]  * m.mat4[5] * m.mat4[14] -
              m.mat4[8]  * m.mat4[6] * m.mat4[13] -
              m.mat4[12] * m.mat4[5] * m.mat4[10] +
              m.mat4[12] * m.mat4[6] * m.mat4[9];

    inv.mat4[1] = -m.mat4[1]  * m.mat4[10] * m.mat4[15] +
             m.mat4[1]  * m.mat4[11] * m.mat4[14] +
             m.mat4[9]  * m.mat4[2] * m.mat4[15] -
             m.mat4[9]  * m.mat4[3] * m.mat4[14] -
             m.mat4[13] * m.mat4[2] * m.mat4[11] +
             m.mat4[13] * m.mat4[3] * m.mat4[10];

    inv.mat4[5] = m.mat4[0]  * m.mat4[10] * m.mat4[15] -
             m.mat4[0]  * m.mat4[11] * m.mat4[14] -
             m.mat4[8]  * m.mat4[2] * m.mat4[15] +
             m.mat4[8]  * m.mat4[3] * m.mat4[14] +
             m.mat4[12] * m.mat4[2] * m.mat4[11] -
             m.mat4[12] * m.mat4[3] * m.mat4[10];

    inv.mat4[9] = -m.mat4[0]  * m.mat4[9] * m.mat4[15] +
             m.mat4[0]  * m.mat4[11] * m.mat4[13] +
             m.mat4[8]  * m.mat4[1] * m.mat4[15] -
             m.mat4[8]  * m.mat4[3] * m.mat4[13] -
             m.mat4[12] * m.mat4[1] * m.mat4[11] +
             m.mat4[12] * m.mat4[3] * m.mat4[9];

    inv.mat4[13] = m.mat4[0]  * m.mat4[9] * m.mat4[14] -
              m.mat4[0]  * m.mat4[10] * m.mat4[13] -
              m.mat4[8]  * m.mat4[1] * m.mat4[14] +
              m.mat4[8]  * m.mat4[2] * m.mat4[13] +
              m.mat4[12] * m.mat4[1] * m.mat4[10] -
              m.mat4[12] * m.mat4[2] * m.mat4[9];

    inv.mat4[2] = m.mat4[1]  * m.mat4[6] * m.mat4[15] -
             m.mat4[1]  * m.mat4[7] * m.mat4[14] -
             m.mat4[5]  * m.mat4[2] * m.mat4[15] +
             m.mat4[5]  * m.mat4[3] * m.mat4[14] +
             m.mat4[13] * m.mat4[2] * m.mat4[7] -
             m.mat4[13] * m.mat4[3] * m.mat4[6];

    inv.mat4[6] = -m.mat4[0]  * m.mat4[6] * m.mat4[15] +
             m.mat4[0]  * m.mat4[7] * m.mat4[14] +
             m.mat4[4]  * m.mat4[2] * m.mat4[15] -
             m.mat4[4]  * m.mat4[3] * m.mat4[14] -
             m.mat4[12] * m.mat4[2] * m.mat4[7] +
             m.mat4[12] * m.mat4[3] * m.mat4[6];

    inv.mat4[10] = m.mat4[0]  * m.mat4[5] * m.mat4[15] -
              m.mat4[0]  * m.mat4[7] * m.mat4[13] -
              m.mat4[4]  * m.mat4[1] * m.mat4[15] +
              m.mat4[4]  * m.mat4[3] * m.mat4[13] +
              m.mat4[12] * m.mat4[1] * m.mat4[7] -
              m.mat4[12] * m.mat4[3] * m.mat4[5];

    inv.mat4[14] = -m.mat4[0]  * m.mat4[5] * m.mat4[14] +
              m.mat4[0]  * m.mat4[6] * m.mat4[13] +
              m.mat4[4]  * m.mat4[1] * m.mat4[14] -
              m.mat4[4]  * m.mat4[2] * m.mat4[13] -
              m.mat4[12] * m.mat4[1] * m.mat4[6] +
              m.mat4[12] * m.mat4[2] * m.mat4[5];

    inv.mat4[3] = -m.mat4[1] * m.mat4[6] * m.mat4[11] +
             m.mat4[1] * m.mat4[7] * m.mat4[10] +
             m.mat4[5] * m.mat4[2] * m.mat4[11] -
             m.mat4[5] * m.mat4[3] * m.mat4[10] -
             m.mat4[9] * m.mat4[2] * m.mat4[7] +
             m.mat4[9] * m.mat4[3] * m.mat4[6];

    inv.mat4[7] = m.mat4[0] * m.mat4[6] * m.mat4[11] -
             m.mat4[0] * m.mat4[7] * m.mat4[10] -
             m.mat4[4] * m.mat4[2] * m.mat4[11] +
             m.mat4[4] * m.mat4[3] * m.mat4[10] +
             m.mat4[8] * m.mat4[2] * m.mat4[7] -
             m.mat4[8] * m.mat4[3] * m.mat4[6];

    inv.mat4[11] = -m.mat4[0] * m.mat4[5] * m.mat4[11] +
              m.mat4[0] * m.mat4[7] * m.mat4[9] +
              m.mat4[4] * m.mat4[1] * m.mat4[11] -
              m.mat4[4] * m.mat4[3] * m.mat4[9] -
              m.mat4[8] * m.mat4[1] * m.mat4[7] +
              m.mat4[8] * m.mat4[3] * m.mat4[5];

    inv.mat4[15] = m.mat4[0] * m.mat4[5] * m.mat4[10] -
              m.mat4[0] * m.mat4[6] * m.mat4[9] -
              m.mat4[4] * m.mat4[1] * m.mat4[10] +
              m.mat4[4] * m.mat4[2] * m.mat4[9] +
              m.mat4[8] * m.mat4[1] * m.mat4[6] -
              m.mat4[8] * m.mat4[2] * m.mat4[5];

    det = m.mat4[0] * inv.mat4[0] + m.mat4[1] * inv.mat4[4] + m.mat4[2] * inv.mat4[8] + m.mat4[3] * inv.mat4[12];

    //if (det == 0)
      //  return false;

    det = 1.0 / det;

    for (i = 0; i < 16; i++)
        out.mat4[i] = inv.mat4[i] * det;

    return (out);
}