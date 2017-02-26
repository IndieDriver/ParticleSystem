#include "Camera.h"

Matrix  get_scale_matrix(float x, float y, float z)
{
    Matrix m;

    m.get_identity();
    m.mat4[0] = x;
    m.mat4[5] = y;
    m.mat4[10] = z;
    return (m);
}

Matrix	get_rot_matrix(float rot_x, float rot_y, float rot_z)
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

Matrix	get_translation_matrix(float x, float y, float z)
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

void	Camera::apply_trans(Vec4 pos, Vec4 rot)
{
    Matrix mtran;
    Matrix mrot;
    Matrix mscale;
    Matrix tmp;

    model.init_matrix();
    mrot = get_rot_matrix(fmod(rot.x, 360.0f), fmod(rot.y, 360.0f), fmod(rot.z, 360.0f));
    mtran = get_translation_matrix(pos.x, pos.y, pos.z);
    mscale = get_scale_matrix(1.0f, 1.0f, 1.0f);
    tmp = mat4_mul(mscale, mrot);
    model = mat4_mul(tmp, mtran);
}

Matrix	get_viewmatrix(Vec4 pos, Vec4 center, Vec4 up)
{
    Matrix  view;
    Vec4    dir;
    Vec4    right;
    Vec4	u;

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
    view.mat4[3] = -dot(right, pos);
    view.mat4[7] = -dot(u, pos);
    view.mat4[11] = -dot(dir, pos);
    return (view);
}

Matrix get_orthomatrix(float left, float right, float top, float bottom, float n, float f)
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

    m.mat4[12] = -(right + left  ) / (right - left  );
    m.mat4[13] = -(top   + bottom) / (top   - bottom);
    m.mat4[14] = -(f   + n ) / (f - n);
    m.mat4[15] = 1.0f;

    return m;
}

Matrix get_projmatrix(float fov, float ratio)
{
    Matrix  mat;
    float	n;
    float	f;
    float	frustum;
    float	oneover;

    n = 0.1f;
    f = 100.0f;
    frustum = n - f;
    oneover = 1.0f / frustum;
    mat.get_identity();

    mat.mat4[5] = 1.0f / tan(fov * ((float)M_PI / 360.0f));
    mat.mat4[0] = (1.0f / tan(fov * ((float)M_PI / 360.0f))) / ratio;
    mat.mat4[10] = (f + n) / frustum;
    mat.mat4[14] = -1.0f;
    mat.mat4[11] = 2.0f * (f * n) / frustum;
    mat.mat4[15] = 0.0f;
    return mat;
}

void Camera::set_mvp()
{
    Matrix tmp;

    tmp = mat4_mul(model, view);
    mvp = mat4_mul(tmp, proj);
}


Camera::Camera(Vec4 pos, Vec4 rot)
{
    apply_trans(pos, rot);

    view = get_viewmatrix(Vec4(0.0f, 0.0f, -1.0f, 0.0f),
                          Vec4(0.0f, 0.0f, 0.0f, 0.0f),
                          Vec4(0.0f, 1.0f, 0.0f, 0.0f));
    //proj = get_projmatrix(45.0f, 1280.0f / 720.0f);
    proj = get_orthomatrix(0.0f, 1280.0f, 720.0f, 0.0f, 0.0f, 200.0f);
    //proj = get_orthomatrix(-1.0f, 1.0f, 1.0f, -1.0f, -1.0f, 1.0f);
    set_mvp();
}