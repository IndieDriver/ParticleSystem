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

Vec4 Matrix::operator*(const Vec4 &rhs) {

	Vec4 result;

    result.x = (mat4[0] * rhs.x) + (mat4[4] * rhs.y)
            + (mat4[8] * rhs.z) + (mat4[12] * rhs.w);
    result.y = (mat4[1] * rhs.x) + (mat4[5] * rhs.y)
            + (mat4[9] * rhs.z) + (mat4[13] * rhs.w);
    result.z = (mat4[2] * rhs.x) + (mat4[6] * rhs.y)
            + (mat4[10] * rhs.z) + (mat4[14] * rhs.w);
    result.w = (mat4[3] * rhs.x) + (mat4[7] * rhs.y)
            + (mat4[11] * rhs.z) + (mat4[15] * rhs.w);
	return (result);
}

Matrix	Matrix::operator*(const Matrix &rhs)
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
                sum += (mat4[i * 4 + k] * rhs.mat4[k * 4 + j]);
                k++;
            }
            res.mat4[i * 4 + j] = sum;
            j++;
        }
        i++;
    }
    return (res);
}

Matrix inverse(const Matrix &m)
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

    det = 1.0f /
          (m.mat4[0] * inv.mat4[0] + m.mat4[1] * inv.mat4[4] + m.mat4[2] * inv.mat4[8] + m.mat4[3] * inv.mat4[12]);
    for (i = 0; i < 16; i++)
        out.mat4[i] = inv.mat4[i] * det;

    return (out);
}


Matrix  scaleMatrix(float x, float y, float z)
{
    Matrix m;

    m.get_identity();
    m.mat4[0] = x;
    m.mat4[5] = y;
    m.mat4[10] = z;
    return (m);
}

Matrix	rotMatrix(float rot_x, float rot_y, float rot_z)
{
    Matrix mat;
    float a;
    float b;
    float c;
    float d;
    float e;

    mat.get_identity();
    a = cos(rot_x);
    b = sin(rot_x);
    c = cos(rot_y);
    d = sin(rot_y);
    e = cos(rot_z);
    mat.mat4[0] = c * e;
    mat.mat4[1] = -c * sin(rot_z);
    mat.mat4[2] = d;
    mat.mat4[4] = (b * d) * e + a * sin(rot_z);
    mat.mat4[5] = -(b * d) * sin(rot_z) + a * e;
    mat.mat4[6] = -b * c;
    mat.mat4[8] = -(a * d) * e + b * sin(rot_z);
    mat.mat4[9] = (a * d) * sin(rot_z) + b * e;
    mat.mat4[10] = a * c;
    mat.mat4[15] = 1.0f;
    return (mat);
}

Matrix	transMatrix(float x, float y, float z)
{
    Matrix mat;

    mat.mat4[0] = 1.0f;
    mat.mat4[5] = 1.0f;
    mat.mat4[10] = 1.0f;
    mat.mat4[15] = 1.0f;
    mat.mat4[12] = x;
    mat.mat4[13] = y;
    mat.mat4[14] = z;
    return (mat);
}

Matrix	modelMatrix(Vec3 pos, Vec3 rot, Vec3 scale)
{
    //ROT MUST BE IN RAD !!
    Matrix mtran;
    Matrix mrot;
    Matrix mscale;
    Matrix tmp;
    Matrix model;

    model.init_matrix();
    //TODO: FIXME
    mrot = rotMatrix(fmod(rot.x, 360.0f), fmod(rot.y, 360.0f), fmod(rot.z, 360.0f));
    mtran = transMatrix(pos.x, pos.y, pos.z);
    mscale = scaleMatrix(scale.x, scale.y, scale.z);
	tmp = mscale * mrot;
	model = tmp * mtran;
    return (model);
}

Matrix	viewMatrix(Vec3 pos, Vec3 center, Vec3 up)
{
    Matrix  view;
    Vec3    dir;
    Vec3    right;
    Vec3	u;


    dir = center - pos;
    dir.normalize();
    u = up;
    u.normalize();
    right = dir.cross(u);
    right.normalize();
    u = right.cross(dir);
    view.get_identity();
    view.mat4[0] = right.x;
    view.mat4[4] = right.y;
    view.mat4[8] = right.z;
    view.mat4[1] = u.x;
    view.mat4[5] = u.y;
    view.mat4[9] = u.z;
    view.mat4[2] = -dir.x;
    view.mat4[6] = -dir.y;
    view.mat4[10] = -dir.z;
    view.mat4[12] = -right.dot(pos);
    view.mat4[13] = - u.dot(pos);
    view.mat4[14] = dir.dot(pos);
    return (view);
}

Matrix orthoMatrix(float left, float right, float top, float bottom, float n, float f)
{
    Matrix m;

    m.mat4[10] = 0.0f;

    m.mat4[0] = 2.0f / (right - left);
    m.mat4[1] = 0.0f;
    m.mat4[2] = 0.0f;
    m.mat4[3] = 0.0f;
    m.mat4[4] = 0.0f;
    m.mat4[5] = 2.0f / (top - bottom);
    m.mat4[6] = 0.0f;
    m.mat4[7] = 0.0f;
    m.mat4[8] = 0.0f;
    m.mat4[9] = 0.0f;
    m.mat4[10] = -2.0f / (f - n);
    m.mat4[11] = 0.0f;
    m.mat4[12] = -(right + left) / (right - left);
    m.mat4[13] = -(top + bottom) / (top - bottom);
    m.mat4[14] = -(f + n) / (f - n);
    m.mat4[15] = 1.0f;
    return m;
}

Matrix projMatrix(float fov, float ratio)
{
    Matrix  mat;
    float	n;
    float	f;
    float	frustum;

    n = 0.1f;
    f = 100.0f;
    frustum = n - f;
    mat.get_identity();
    // NOT RAD
    float tanHalfFov = tanf(fov / 2.0f);

    mat.mat4[0] = 1.0f / (ratio * tanHalfFov);
    mat.mat4[5] = 1.0f / tanHalfFov;
    mat.mat4[10] = (f + n) / frustum;
    mat.mat4[11] = -1.0f;
    mat.mat4[14] = 2.0f * (f * n) / frustum;
    mat.mat4[15] = 0.0f;
    return (mat);
}

void printMatrix(float array[16])
{
    int i;

    i = 0;
    while (i < 16)
    {
        printf("%f ", array[i]);
        if ((i + 1) % 4 == 0)
            printf("\n");
        i++;
    }
    printf("\n");
}
