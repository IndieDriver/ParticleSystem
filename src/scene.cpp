#include "scene.hpp"

Scene::Scene(CLenv *clenv, Camera *cam, unsigned int particle_nb)
    : _cl(clenv),
      _camera(cam),
      _num_particle(particle_nb),
      _model(ModelType::Sphere) {
  vao = 0;
  glGenVertexArrays(1, &vao);
  glBindVertexArray(vao);

  glBindBuffer(GL_ARRAY_BUFFER, _cl->pos_id);
  glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 0, NULL);

  glEnableVertexAttribArray(0);
}

void Scene::draw(const Shader &shader) {
  GLint mvpID = glGetUniformLocation(shader.id, "MVP");
  GLint cursorPosID = glGetUniformLocation(shader.id, "cursorPos");
  glm::mat4 model(1.0f);
  glm::mat4 MVP = _camera->proj * _camera->view * model;
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  shader.use();
  glUniformMatrix4fv(glGetUniformLocation(shader.id, "MVP"), 1, GL_FALSE,
                     glm::value_ptr(MVP));
  glUniform3fv(glGetUniformLocation(shader.id, "cursorPos"), 1,
               &(lastCursorPos.x));

  glBindVertexArray(vao);
  glDrawArrays(GL_POINTS, 0, static_cast<GLsizei>(_num_particle));
}

void Scene::initScene(const Env &env) {
  try {
    if (shouldUpdateCursorPos) {
      lastCursorPos = getCursorPosInWorldSpace(env);
    }
    lastCursorPos.w = static_cast<float>(_model);
    _state = SceneState::Running;
    glFlush();
    glFinish();
    int status = 0;
    std::vector<cl::Memory> cl_vbos;
    cl_vbos.push_back(_cl->buf_pos);
    _cl->cmds.enqueueAcquireGLObjects(&cl_vbos, NULL, NULL);
    _cl->enqueueKernel(_cl->kinit, lastCursorPos, _num_particle, 0.0f);
    status = _cl->cmds.finish();
    if (status < 0) printf("Error clfinish\n");
    _cl->cmds.enqueueReleaseGLObjects(&cl_vbos, NULL, NULL);
  } catch (cl::Error e) {
    std::cout << std::endl << e.what() << " : Error " << e.err() << std::endl;
  }
}

void Scene::animate(const Env &env, float deltaTime) {
  if (shouldUpdateCursorPos) {
    lastCursorPos = getCursorPosInWorldSpace(env);
  }

  lastCursorPos.w = gravity ? 0.0f : -1.0f;
  try {
    glFlush();
    glFinish();
    int status = 0;
    std::vector<cl::Memory> cl_vbos;
    cl_vbos.push_back(_cl->buf_pos);
    status = _cl->cmds.enqueueAcquireGLObjects(&cl_vbos, NULL, NULL);
    _cl->enqueueKernel(_cl->kernel, lastCursorPos, _num_particle, deltaTime);
    status = _cl->cmds.finish();
    if (status < 0) printf("Error clfinish\n");
    _cl->cmds.enqueueReleaseGLObjects(&cl_vbos, NULL, NULL);
  } catch (cl::Error e) {
    std::cout << std::endl << e.what() << " : Error " << e.err() << std::endl;
  }
}

cl_float4 Scene::getCursorPosInWorldSpace(const Env &env) {
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
      cl_float4 result;
      result.s[0] = _camera->pos.x + rayWorld.x * t;
      result.s[1] = _camera->pos.y + rayWorld.y * t;
      result.s[2] = _camera->pos.z + rayWorld.z * t;
      result.s[3] = 1.0f;
      return (result);
    } else {
      std::cout << "invalid t" << std::endl;
    }
  } else {
    std::cout << "invalid dot" << std::endl;
  }

  cl_float4 zero;
  zero.s[0] = 0.0f;
  zero.s[1] = 0.0f;
  zero.s[2] = 0.0f;
  zero.s[3] = 0.0f;

  return (zero);
}

void Scene::update(Env &env) {
  if (env.inputHandler.keys[GLFW_KEY_SPACE]) {
    env.inputHandler.keys[GLFW_KEY_SPACE] = false;
    gravity = !gravity;
  }
  if (env.inputHandler.keys[GLFW_KEY_F]) {
    env.inputHandler.keys[GLFW_KEY_F] = false;
    isFreeCam = !isFreeCam;
    env.setCursorLock(isFreeCam);
    shouldUpdateCursorPos = false;
  }
  if (env.inputHandler.keys[GLFW_KEY_J]) {
    env.inputHandler.keys[GLFW_KEY_J] = false;
    switch (_model) {
      case ModelType::Sphere:
        _model = ModelType::Cube;
        break;
      case ModelType::Cube:
        _model = ModelType::Sphere;
        break;
      default:
        break;
    }
    gravity = false;
    _state = SceneState::Init;
    // needInit = true;
  }
  if (env.inputHandler.keys[GLFW_KEY_G]) {
    env.inputHandler.keys[GLFW_KEY_G] = false;
    shouldUpdateCursorPos = !shouldUpdateCursorPos;
  }
  if (_state == SceneState::Init) {
    initScene(env);
  } else {
    animate(env, env.getDeltaTime());
  }
}
