#ifndef PARTICLESYSTEM_CAMERA_H
#define PARTICLESYSTEM_CAMERA_H

#include "part.h"
#include "Matrix.h"


class Camera {
public:
    Matrix model;
    Matrix view;
    Matrix proj;
    Matrix mvp;
    GLint mvp_id;
    Vec4   pos;
    Vec4   rot;
    Camera(Vec4 pos, Vec4 rot);
    void apply_trans(Vec4 pos, Vec4 rot);
    void set_mvp();
};


#endif //PARTICLESYSTEM_CAMERA_H
