#pragma once
#include <iomanip>
#include <unordered_map>
#include "camera.hpp"
#include "env.hpp"
#include "part.hpp"
#include "renderer.hpp"
#include "shader.hpp"

enum class ModelType { Sphere, Cube };

enum class SceneState { Init, Running, Gravity };

enum class ShaderType { Normal, Billboard };

class Scene {
 public:
  glm::vec4 last_cursor_pos;
  Scene(CLenv *env, Camera *camera, unsigned int num_particle);
  ~Scene();
  void draw(const Env &env);
  void updateBuffer(const Cglbuffer &buffer, const Env &env);
  void invokeKernel(const cl::Kernel kernel, const Cglbuffer &buffer,
                    const Env &env);
  void addEmitedParticles(unsigned int num_particle, const Env &env);

  void update(Env &env);
  glm::vec4 getCursorPosInWorldSpace(const Env &env);

 private:
  void print_debug_info(const Env &env);
  void updateState(Env &env);
  CLenv *_cl;
  Camera *_camera;
  Shader *_shader;
  Shader *_billboard_shader;
  Texture *_billboard_texture = nullptr;
  Cglbuffer _main_buffers;
  Renderer _renderer;
  std::unordered_map<float, Cglbuffer> _emit_buffers;
  unsigned int _num_particle;
  enum ModelType _model;
  enum SceneState _state;
  enum ShaderType _shader_type;
  bool gravity = false;
  bool free_cam = false;
  bool tracking_cursor_pos = true;
  bool debug_mode = false;
};

static std::string float_to_string(float f, int prec) {
  std::ostringstream out;
  out << std::setprecision(prec) << std::fixed << f;
  return out.str();
}
