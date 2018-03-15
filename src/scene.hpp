#pragma once
#include "CLenv.h"
#include "Shader.h"
#include "camera.hpp"
#include "env.hpp"
#include "part.h"

class Scene {
 public:
  GLuint vao;
  bool needInit = true;
  bool isFreeCam = false;
  bool gravity = false;
  bool sphere = true;
  bool shouldUpdateCursorPos = true;
  cl_float4 lastCursorPos;
  Scene(CLenv *env, Camera *camera, unsigned int num_particle);
  void draw(const Shader &shader);
  void initScene(const Env &env);
  void animate(const Env &env, float deltaTime);
  void update(Env &env);
  cl_float4 getCursorPosInWorldSpace(float mouse_x, float mouse_y);

 private:
  CLenv *_cl;
  Camera *_camera;
  unsigned int _num_particle;
};
