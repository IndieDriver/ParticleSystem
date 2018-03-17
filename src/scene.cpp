#include "scene.hpp"

Scene::Scene(CLenv *clenv, Camera *cam, unsigned int particle_nb)
    : _cl(clenv),
      _camera(cam),
      _num_particle(particle_nb),
      _model(ModelType::Sphere) {
  _shader = new Shader("shaders/part.frag", "shaders/part.vert");
  _billboard_shader =
      new Shader("shaders/bill_part.frag", "shaders/bill_part.vert",
                 "shaders/bill_part.geom");
  try {
    _billboard_texture = new Texture("textures/cloud2.png");
  } catch (std::exception &e) {
    std::cerr << e.what() << std::endl;
  }
  GLuint vao;
  glGenVertexArrays(1, &vao);
  glBindVertexArray(vao);

  _main_buffers = _cl->createGLBuffer(particle_nb * 4 * sizeof(float));
  _main_buffers.vao = vao;
  _main_buffers.size = particle_nb;

  glBindBuffer(GL_ARRAY_BUFFER, _main_buffers.vbo);
  glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 0, NULL);

  glEnableVertexAttribArray(0);
}

Scene::~Scene() {
  if (_main_buffers.vbo != 0) {
    glDeleteBuffers(1, &_main_buffers.vbo);
  }
  if (_main_buffers.vao != 0) {
    glDeleteVertexArrays(1, &_main_buffers.vao);
  }
  delete _shader;
  delete _billboard_shader;
  delete _billboard_texture;
}

void Scene::draw(const Env &env) {
  Shader *shader = nullptr;
  switch (_shader_type) {
    case ShaderType::Normal:
      shader = _shader;
      break;
    case ShaderType::Billboard:
      shader = _billboard_shader;
      break;
    default:
      break;
  }
  if (shader == nullptr) {
    return;
  }
  GLint mvpID = glGetUniformLocation(shader->id, "MVP");
  GLint cursorPosID = glGetUniformLocation(shader->id, "cursorPos");
  glm::mat4 model(1.0f);
  glm::mat4 MVP = _camera->proj * _camera->view * model;
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  shader->use();
  glUniformMatrix4fv(glGetUniformLocation(shader->id, "MVP"), 1, GL_FALSE,
                     glm::value_ptr(MVP));
  glUniform3fv(glGetUniformLocation(shader->id, "cursorPos"), 1,
               glm::value_ptr(last_cursor_pos));
  glUniform1f(glGetUniformLocation(shader->id, "iTime"), env.getAbsoluteTime());
  if (_shader_type == ShaderType::Billboard && _billboard_texture != nullptr) {
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, _billboard_texture->id);
  }

  glBindVertexArray(_main_buffers.vao);
  glDrawArrays(GL_POINTS, 0, _main_buffers.size);

  for (const auto &buffer : _emit_buffers) {
    glBindVertexArray(buffer.second.vao);
    glDrawArrays(GL_POINTS, 0, buffer.second.size);
  }
  if (debug_mode) {
    print_debug_info(env);
  }
}

inline cl_float4 vec4_to_clfloat4(glm::vec4 glm_vec) {
  cl_float4 vec;
  vec.x = glm_vec.x;
  vec.y = glm_vec.y;
  vec.z = glm_vec.z;
  vec.w = glm_vec.w;
  return (vec);
}

void Scene::initScene(const Cglbuffer &buffer, const Env &env) {
  try {
    if (tracking_cursor_pos) {
      last_cursor_pos = getCursorPosInWorldSpace(env);
    }
    last_cursor_pos.w = static_cast<float>(_model);
    _state = SceneState::Running;
    glFlush();
    glFinish();
    int status = 0;
    std::vector<cl::Memory> cl_vbos;
    cl_vbos.push_back(buffer.position);
    _cl->cmds.enqueueAcquireGLObjects(&cl_vbos, NULL, NULL);
    _cl->enqueueKernel(_cl->kinit, buffer, vec4_to_clfloat4(last_cursor_pos),
                       buffer.size, 0.0f);
    status = _cl->cmds.finish();
    if (status < 0) printf("Error clfinish\n");
    _cl->cmds.enqueueReleaseGLObjects(&cl_vbos, NULL, NULL);
  } catch (cl::Error e) {
    std::cout << std::endl << e.what() << " : Error " << e.err() << std::endl;
  }
}
void Scene::invokeKernel(const cl::Kernel kernel, const Cglbuffer &buffer,
                         const Env &env) {
  if (tracking_cursor_pos) {
    last_cursor_pos = getCursorPosInWorldSpace(env);
  }

  last_cursor_pos.w = gravity ? 0.0f : -1.0f;
  try {
    glFlush();
    glFinish();
    int status = 0;
    std::vector<cl::Memory> cl_vbos;
    cl_vbos.push_back(buffer.position);
    status = _cl->cmds.enqueueAcquireGLObjects(&cl_vbos, NULL, NULL);
    _cl->enqueueKernel(kernel, buffer, vec4_to_clfloat4(last_cursor_pos),
                       buffer.size, env.getDeltaTime());
    status = _cl->cmds.finish();
    if (status < 0) printf("Error clfinish\n");
    _cl->cmds.enqueueReleaseGLObjects(&cl_vbos, NULL, NULL);
  } catch (cl::Error e) {
    std::cout << std::endl << e.what() << " : Error " << e.err() << std::endl;
  }
}

glm::vec4 Scene::getCursorPosInWorldSpace(const Env &env) {
  float x = (2.0f * env.inputHandler.mousex) / env.width - 1.0f;
  float y = 1.0f - (2.0f * env.inputHandler.mousey) / env.height;
  glm::vec4 rayClip(x, y, -1.0f, 1.0f);

  glm::mat4 invProj = glm::inverse(_camera->proj);
  glm::vec4 rayEye = invProj * rayClip;
  rayEye = glm::vec4(rayEye.x, rayEye.y, -1.0f, 0.0f);
  glm::mat4 invView = inverse(_camera->view);

  glm::vec4 rayWorld4 = invView * rayEye;

  glm::vec3 rayWorld(rayWorld4);
  rayWorld = glm::normalize(rayWorld);

  glm::vec3 planNormal = _camera->dir;
  planNormal = glm::normalize(planNormal);

  glm::vec3 planPos = _camera->pos + rayWorld * 10.0f;

  float denom = glm::dot(planNormal, rayWorld);
  if (fabs(denom) > 1e-6) {
    float t = glm::dot(planPos - _camera->pos, rayWorld) / denom;
    if (t >= 1e-6) {
      return (glm::vec4(_camera->pos + rayWorld * t, 1.0f));
    }
  }
  return (glm::vec4(0.0f));
}

void Scene::update(Env &env) {
  _renderer.update(env);
  if (env.inputHandler.keys[GLFW_KEY_SPACE]) {
    env.inputHandler.keys[GLFW_KEY_SPACE] = false;
    gravity = !gravity;
  }
  if (env.inputHandler.keys[GLFW_KEY_F]) {
    env.inputHandler.keys[GLFW_KEY_F] = false;
    free_cam = !free_cam;
    env.setCursorLock(free_cam);
    tracking_cursor_pos = false;
  }
  if (env.inputHandler.keys[GLFW_KEY_E]) {
    env.inputHandler.keys[GLFW_KEY_E] = false;
    Cglbuffer buffer;
    GLuint vao;
    glGenVertexArrays(1, &vao);
    glBindVertexArray(vao);

    buffer = _cl->createGLBuffer(EMIT_PARTICLE * 4 * sizeof(float));
    buffer.vao = vao;
    buffer.size = EMIT_PARTICLE;

    glBindBuffer(GL_ARRAY_BUFFER, buffer.vbo);
    glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 0, NULL);

    glEnableVertexAttribArray(0);
    _emit_buffers.emplace(env.getAbsoluteTime(), buffer);
    invokeKernel(_cl->kemit, buffer, env);
  }
  if (env.inputHandler.keys[GLFW_KEY_1]) {
    env.inputHandler.keys[GLFW_KEY_1] = false;
    _model = ModelType::Sphere;
    gravity = false;
    _state = SceneState::Init;
  }
  if (env.inputHandler.keys[GLFW_KEY_2]) {
    env.inputHandler.keys[GLFW_KEY_2] = false;
    _model = ModelType::Cube;
    gravity = false;
    _state = SceneState::Init;
  }
  if (env.inputHandler.keys[GLFW_KEY_Z]) {
    env.inputHandler.keys[GLFW_KEY_Z] = false;
    if (_state == SceneState::Running) {
      _state = SceneState::Gravity;
    } else {
      _state = SceneState::Running;
    }
  }
  if (env.inputHandler.keys[GLFW_KEY_G]) {
    env.inputHandler.keys[GLFW_KEY_G] = false;
    tracking_cursor_pos = !tracking_cursor_pos;
  }
  if (env.inputHandler.keys[GLFW_KEY_I]) {
    env.inputHandler.keys[GLFW_KEY_I] = false;
    debug_mode = !debug_mode;
  }
  if (env.inputHandler.keys[GLFW_KEY_Q]) {
    env.inputHandler.keys[GLFW_KEY_Q] = false;
    switch (_shader_type) {
      case ShaderType::Normal:
        _shader_type = ShaderType::Billboard;
        break;
      case ShaderType::Billboard:
        _shader_type = ShaderType::Normal;
        break;
      default:
        break;
    }
  }
  float current_time = env.getAbsoluteTime();
  for (auto it = _emit_buffers.begin(); it != _emit_buffers.end();) {
    if (current_time - it->first > 10.0f) {
      it = _emit_buffers.erase(it);
    } else {
      ++it;
    }
  }
  switch (_state) {
    case SceneState::Init:
      initScene(_main_buffers, env);
      break;
    case SceneState::Running:
      invokeKernel(_cl->kernel, _main_buffers, env);
      break;
    case SceneState::Gravity:
      invokeKernel(_cl->kgravity, _main_buffers, env);
      break;
    default:
      break;
  }
  for (const auto &buffer : _emit_buffers) {
    switch (_state) {
      case SceneState::Init:
        initScene(buffer.second, env);
        break;
      case SceneState::Running:
        invokeKernel(_cl->kernel, buffer.second, env);
        break;
      case SceneState::Gravity:
        invokeKernel(_cl->kgravity, buffer.second, env);
        break;
      default:
        break;
    }
  }
}

std::string float_to_string(float f, int prec) {
  std::ostringstream out;
  out << std::setprecision(prec) << std::fixed << f;
  return out.str();
}

void Scene::print_debug_info(const Env &env) {
  float fheight = static_cast<float>(_renderer.getScreenHeight());
  float fwidth = static_cast<float>(_renderer.getScreenWidth());
  size_t particle_count = _main_buffers.size;
  for (const auto &buffer : _emit_buffers) {
    particle_count += buffer.second.size;
  }
  _renderer.renderText(10.0f, fheight - 25.0f, 0.35f,
                       "x: " + float_to_string(_camera->pos.x, 2) +
                           " y: " + float_to_string(_camera->pos.y, 2) +
                           " z: " + float_to_string(_camera->pos.z, 2),
                       glm::vec3(1.0f, 1.0f, 1.0f));
  _renderer.renderText(
      10.0f, fheight - 50.0f, 0.35f,
      "vel: " + float_to_string(_camera->velocity, 2) + " m/s ",
      glm::vec3(1.0f, 1.0f, 1.0f));
  _renderer.renderText(10.0f, fheight - 75.0f, 0.35f,
                       std::to_string(particle_count) + " particles",
                       glm::vec3(1.0f, 1.0f, 1.0f));
  _renderer.renderText(fwidth - 130.0f, fheight - 25.0f, 0.35f,
                       float_to_string(env.getFPS(), 2) + " fps",
                       glm::vec3(1.0f, 1.0f, 1.0f));
}
