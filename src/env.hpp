#pragma once
#include <array>
#include <iostream>
#include <sstream>
#include <string>
#include "part.hpp"

#include <glad/glad.h>

#include <OpenCL/cl.h>
#include <OpenCL/cl_gl.h>

#include <OpenGL/CGLCurrent.h>
#include <OpenGL/CGLDevice.h>
#include <CL/cl.hpp>

#include <GLFW/glfw3.h>

enum class mouseState { VIRTUAL, NORMAL, HIDDEN };

class InputHandler {
 public:
  std::array<bool, 1024> keys = {{0}};
  bool mouselocked = false;
  // bool mouseDisabled = true;
  bool keybrDisabled = true;
  float mousex;
  float mousey;
  std::array<bool, 12> mouse_keys = {{0}};
};

class Env {
 public:
  GLFWwindow *window;
  InputHandler inputHandler;
  Env();
  Env(unsigned short width, unsigned short height);
  ~Env();

  void toggleFullscreen();
  void update();  // Called once per frame
  float getDeltaTime() const;
  float getAbsoluteTime() const;
  float getFrame() const;
  float getFPS() const;
  void setCursorLock(bool l);
  int width;
  int height;
  bool has_resized;

 private:
  void updateFpsCounter();
  void setDeltaTime();
  void setupContext();
  void setupWindow();
  void setupWindowHint();
  float _absoluteTime;
  float _deltaTime;
  float _frame;
  float _fps;
  bool _fullscreen = false;
  int _window_width;
  int _window_height;
};

class CLenv {
 public:
  CLenv(std::string file_name);
  void createBuffer(int num_particle);
  void enqueueKernel(cl::Kernel kernel, cl_float4 cursor, int num_particle,
                     float deltaTime);
  cl::Device id;
  cl::Context context;
  cl::CommandQueue cmds;
  cl::Program program;
  cl::Kernel kinit;
  cl::Kernel kernel;
  GLuint pos_id;
  GLuint col_id;
  cl::Buffer buf_vel;
  cl::BufferGL buf_pos;
  cl::BufferGL buf_col;

  bool loadProgram(std::string filename, cl::Device device);
};

void keyCallback(GLFWwindow *window, int key, int scancode, int action,
                 int mode);
void mouseCallback(GLFWwindow *window, double xpos, double ypos);
void mouseKeyCallback(GLFWwindow *window, int button, int action, int mods);

static void GL_DUMP_ERROR(std::string message) {
  int glErrorCode = 0;
  if ((glErrorCode = glGetError()) != GL_NO_ERROR) {
    std::string error;
    switch (glErrorCode) {
      case GL_INVALID_ENUM:
        error = "INVALID_ENUM";
        break;
      case GL_INVALID_VALUE:
        error = "INVALID_VALUE";
        break;
      case GL_INVALID_OPERATION:
        error = "INVALID_OPERATION";
        break;
      case GL_STACK_OVERFLOW:
        error = "STACK_OVERFLOW";
        break;
      case GL_STACK_UNDERFLOW:
        error = "STACK_UNDERFLOW";
        break;
      case GL_OUT_OF_MEMORY:
        error = "OUT_OF_MEMORY";
        break;
      case GL_INVALID_FRAMEBUFFER_OPERATION:
        error = "INVALID_FRAMEBUFFER_OPERATION";
        break;
    }
    std::cout << message << " (" << glErrorCode << ": " << error << ")\n";
  }
}
