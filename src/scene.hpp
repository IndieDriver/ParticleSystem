#pragma once
#include "camera.hpp"
#include "env.hpp"
#include "part.hpp"
#include "shader.hpp"

enum class ModelType { Sphere, Cube };

enum class SceneState { Init, Running };

class Scene {
 public:
  GLuint vao;
  bool isFreeCam = false;
  bool gravity = false;
  bool shouldUpdateCursorPos = true;
  cl_float4 lastCursorPos;
  Scene(CLenv *env, Camera *camera, unsigned int num_particle);
  void draw(const Shader &shader);
  void initScene(const Env &env);
  void animate(const Env &env, float deltaTime);
  void update(Env &env);
  cl_float4 getCursorPosInWorldSpace(const Env &env);

 private:
  CLenv *_cl;
  Camera *_camera;
  unsigned int _num_particle;
  enum ModelType _model;
  enum SceneState _state;
};
