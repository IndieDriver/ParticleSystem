#pragma once
#include <unordered_map>
#include "camera.hpp"
#include "env.hpp"
#include "part.hpp"
#include "shader.hpp"

enum class ModelType { Sphere, Cube };

enum class SceneState { Init, Running };

class Scene {
 public:
  glm::vec4 last_cursor_pos;
  Scene(CLenv *env, Camera *camera, unsigned int num_particle);
  ~Scene();
  void draw(const Env &env, const Shader &shader);
  void initScene(const Cglbuffer &buffer, const Env &env);
  void animate(const Cglbuffer &buffer, const Env &env);
  void emit(const Cglbuffer &buffer, const Env &env);
  void update(Env &env);
  glm::vec4 getCursorPosInWorldSpace(const Env &env);

 private:
  CLenv *_cl;
  Camera *_camera;
  Cglbuffer _main_buffers;
  std::unordered_map<float, Cglbuffer> _emit_buffers;
  unsigned int _num_particle;
  enum ModelType _model;
  enum SceneState _state;
  bool gravity = false;
  bool free_cam = false;
  bool tracking_cursor_pos = true;
};
