#include "env.hpp"

Env::Env() : Env(0, 0) {}

Env::Env(unsigned short width, unsigned short height)
    : width(width),
      height(height),
      _frame(0),
      _window_width(1280),
      _window_height(720) {
  if (!glfwInit()) return;
  setupWindowHint();
  GLFWmonitor *primary_monitor = glfwGetPrimaryMonitor();
  const GLFWvidmode *mode = glfwGetVideoMode(primary_monitor);
  if (width == 0 && height == 0) {
    this->width = mode->width;
    this->height = mode->height;
    window = glfwCreateWindow(this->width, this->height, "ParticleSystem",
                              primary_monitor, NULL);
    glfwSetWindowMonitor(window, primary_monitor, 0, 0, mode->width,
                         mode->height, mode->refreshRate);
  } else {
    _window_width = width;
    _window_height = height;
    window = glfwCreateWindow(width, height, "ft_vox", NULL, NULL);
    glfwSetWindowMonitor(window, NULL, (mode->width / 2) - (_window_width / 2),
                         (mode->height / 2) - (_window_height / 2),
                         _window_width, _window_height, 0);
    inputHandler.mousex = _window_width / 2;
    inputHandler.mousey = _window_height / 2;
  }
  if (!window) {
    std::cout << "Could not create window\n";
    glfwTerminate();
    return;
  }
  glfwMakeContextCurrent(window);
  if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
    std::cout << "Failed to initialize OpenGL context" << std::endl;
  }
  std::cout << "OpenGL " << glGetString(GL_VERSION) << std::endl;
  std::cout << "GLSL " << glGetString(GL_SHADING_LANGUAGE_VERSION) << std::endl;

  setupWindow();
  setupContext();
}

Env::~Env() { glfwTerminate(); }

void Env::toggleFullscreen() {
  _fullscreen = !_fullscreen;
  GLFWmonitor *primary_monitor = glfwGetPrimaryMonitor();
  const GLFWvidmode *mode = glfwGetVideoMode(primary_monitor);
  if (_fullscreen) {
    glfwSetWindowMonitor(window, primary_monitor, 0, 0, mode->width,
                         mode->height, mode->refreshRate);
    this->width = mode->width;
    this->height = mode->height;
    // The mouse virtual position reported by curposPosCallback promptly jump
    // after a window -> fullscreen transition
    // Doesn't happen on fullscreen -> window
    // GLFW bug ?
    inputHandler.mousex -= (this->_window_width / 2);
    inputHandler.mousey -= (this->_window_height / 2);
    this->has_resized = true;
  } else {
    glfwSetWindowMonitor(window, NULL, (mode->width / 2) - (_window_width / 2),
                         (mode->height / 2) - (_window_height / 2),
                         _window_width, _window_height, 0);
    this->width = _window_width;
    this->height = _window_height;
  }
  // Query and update framebuffer size
  int wframe, hframe;
  glfwGetFramebufferSize(window, &wframe, &hframe);
  glViewport(0, 0, wframe, hframe);
}

void Env::setupWindowHint() {
  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 1);
  glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
  glfwWindowHint(GLFW_DECORATED, GLFW_TRUE);
}

void Env::setupWindow() {
  if (window != nullptr) {
    glfwSetWindowUserPointer(window, &inputHandler);
    if (inputHandler.mouselocked) {
      glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    } else {
      glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
    }
    glfwSetCursorPosCallback(window, mouseCallback);
    glfwSetKeyCallback(window, keyCallback);
    glfwSetMouseButtonCallback(window, mouseKeyCallback);
  }
}

void Env::setupContext() {
  glfwSwapInterval(0);
  glEnable(GL_DEBUG_OUTPUT);
  while (glGetError() != GL_NO_ERROR)
    ;  // Flush gl_error
  glEnable(GL_DEPTH_TEST);
  glDepthFunc(GL_LESS);
  // glEnable(GL_BLEND);
  // glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
  // glEnable(GL_CULL_FACE);
  // glEnable(GL_FRAMEBUFFER_SRGB);
}

void Env::update() {
  updateFpsCounter();
  static double previousTime = glfwGetTime();
  static int frame_count;
  double currentTime = glfwGetTime();
  double deltaTime = currentTime - previousTime;
  previousTime = currentTime;
  this->_deltaTime = static_cast<float>(deltaTime);
  this->_absoluteTime = static_cast<float>(currentTime);
  this->_frame++;
}

void Env::updateFpsCounter() {
  static double previous_seconds = glfwGetTime();
  static int frame_count;
  double current_seconds = glfwGetTime();
  double elapsed_seconds = current_seconds - previous_seconds;
  if (elapsed_seconds > 1.0) {
    previous_seconds = current_seconds;
    this->_fps =
        static_cast<float>(frame_count) / static_cast<float>(elapsed_seconds);
    std::ostringstream tmp;
    tmp << _fps << " fps";
    glfwSetWindowTitle(window, tmp.str().c_str());
    frame_count = 0;
  }
  frame_count++;
}

float Env::getDeltaTime() const { return (this->_deltaTime); }

float Env::getAbsoluteTime() const { return (this->_absoluteTime); }

float Env::getFrame() const { return (this->_frame); }

float Env::getFPS() const { return (this->_fps); }

void Env::setCursorLock(bool l) {
  this->inputHandler.mouselocked = l;
  if (inputHandler.mouselocked) {
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
  } else {
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
  }
}

void keyCallback(GLFWwindow *window, int key, int scancode, int action,
                 int mode) {
  (void)scancode;
  (void)mode;
  InputHandler *inputHandler =
      reinterpret_cast<InputHandler *>(glfwGetWindowUserPointer(window));
  if (action == GLFW_PRESS) {
    inputHandler->keys[key] = true;
  } else if (action == GLFW_RELEASE) {
    inputHandler->keys[key] = false;
  }
}

void mouseCallback(GLFWwindow *window, double xpos, double ypos) {
  InputHandler *inputHandler =
      reinterpret_cast<InputHandler *>(glfwGetWindowUserPointer(window));
  inputHandler->mousex = static_cast<float>(xpos);
  inputHandler->mousey = static_cast<float>(ypos);
}

void mouseKeyCallback(GLFWwindow *window, int button, int action, int mods) {
  (void)mods;
  InputHandler *inputHandler =
      reinterpret_cast<InputHandler *>(glfwGetWindowUserPointer(window));
  if (action == GLFW_PRESS) {
    inputHandler->mouse_keys[button] = true;
  } else if (action == GLFW_RELEASE) {
    inputHandler->mouse_keys[button] = false;
  }
}

void CLenv::enqueueKernel(cl::Kernel k, cl_float4 cursorpos, int num_particle,
                          float deltaTime) {
  try {
    k.setArg(0, sizeof(cl_float), &deltaTime);
    k.setArg(1, sizeof(cl_float4), &cursorpos);
    k.setArg(2, sizeof(cl_mem), &buf_pos);
    k.setArg(3, sizeof(cl_mem), &buf_vel);
    cmds.enqueueNDRangeKernel(k, 0, cl::NDRange(num_particle));
  } catch (cl::Error e) {
    std::cout << std::endl << e.what() << " : Error " << e.err() << std::endl;
  }
}

void CLenv::createBuffer(int num_particle) {
  try {
    glGenBuffers(1, &pos_id);
    glBindBuffer(GL_ARRAY_BUFFER, pos_id);
    glBufferData(GL_ARRAY_BUFFER, 4 * num_particle * sizeof(float), NULL,
                 GL_STATIC_DRAW);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    int status = 0;

    buf_vel =
        cl::Buffer(this->context, CL_MEM_READ_WRITE | CL_MEM_ALLOC_HOST_PTR,
                   4 * sizeof(float) * num_particle, NULL, &status);

    if (status < 0) std::cout << status << "buffer error\n";
    // status = cmds.enqueueWriteBuffer(buf_vel, CL_TRUE, 0, 4 * sizeof(float) *
    // num_particle, NULL);
    if (status < 0) std::cout << status << "enqueue error\n";
    buf_pos = cl::BufferGL(this->context, CL_MEM_READ_WRITE, pos_id);
  } catch (cl::Error e) {
    std::cout << std::endl << e.what() << " : Error " << e.err() << std::endl;
  }
}

CLenv::CLenv(std::string kernelFileName) {
  try {
    std::vector<cl::Platform> all_platforms;
    cl::Platform::get(&all_platforms);
    if (all_platforms.size() == 0) {
      std::cout << " No platforms found. Check OpenCL installation!\n";
      exit(1);
    }
    cl::Platform default_platform = all_platforms[0];
    std::cout << "OpenCL " << default_platform.getInfo<CL_PLATFORM_VERSION>()
              << std::endl;
    std::vector<cl::Device> all_devices;
    default_platform.getDevices(CL_DEVICE_TYPE_GPU, &all_devices);
    const cl::Device default_device = all_devices[0];
#ifdef linux
    cl_context_properties properties[] = {
        CL_GL_CONTEXT_KHR,
        (cl_context_properties)glXGetCurrentContext(),
        CL_GLX_DISPLAY_KHR,
        (cl_context_properties)glXGetCurrentDisplay(),
        CL_CONTEXT_PLATFORM,
        (cl_context_properties)(default_platform)(),
        0};
#elif defined _WIN32
    cl_context_properties properties[] = {
        CL_GL_CONTEXT_KHR,
        (cl_context_properties)wglGetCurrentContext(),
        CL_WGL_HDC_KHR,
        (cl_context_properties)wglGetCurrentDC(),
        CL_CONTEXT_PLATFORM,
        (cl_context_properties)(default_platform)(),
        0};
#elif defined __APPLE__
    CGLContextObj glContext = (CGLContextObj)CGLGetCurrentContext();
    CGLShareGroupObj shareGroup = CGLGetShareGroup(glContext);
    cl_context_properties properties[] = {
        CL_CONTEXT_PROPERTY_USE_CGL_SHAREGROUP_APPLE,
        (cl_context_properties)shareGroup, 0};
#endif

    context = cl::Context(default_device, properties);
    cmds = cl::CommandQueue(context, default_device);
    if (loadProgram(kernelFileName, default_device)) {
      kinit = cl::Kernel(program, "clinit");
      kernel = cl::Kernel(program, "clpart");
    }
  } catch (cl::Error e) {
    std::cout << std::endl << e.what() << " : " << e.err() << std::endl;
  }
}

bool CLenv::loadProgram(std::string filename, cl::Device device) {
  std::string line;
  std::ifstream file(filename);
  std::string buffer = "";

  if (file) {
    while (getline(file, line)) buffer += line + "\n";
    file.close();
  }
  cl::Program::Sources source(1,
                              std::make_pair(buffer.c_str(), buffer.length()));
  program = cl::Program(context, source);
  try {
    program.build({device});
  } catch (cl::Error e) {
    cl_build_status status =
        program.getBuildInfo<CL_PROGRAM_BUILD_STATUS>(device);
    std::string name = device.getInfo<CL_DEVICE_NAME>();
    std::string buildlog = program.getBuildInfo<CL_PROGRAM_BUILD_LOG>(device);
    std::cerr << "Build log for " << name << ":" << std::endl
              << buildlog << std::endl;
    return (false);
  }
  return (true);
}

void debugPrintAllDevice() {
  std::vector<cl::Platform> all_platforms;
  cl::Platform::get(&all_platforms);
  if (all_platforms.size() == 0) {
    std::cout << " No platforms found. Check OpenCL installation!\n";
    exit(1);
  }
  for (unsigned i = 0; i < all_platforms.size(); i++)
    std::cout << ' ' << all_platforms.at(i).getInfo<CL_PLATFORM_NAME>()
              << " with extension:"
              << all_platforms.at(i).getInfo<CL_PLATFORM_EXTENSIONS>()
              << "\n\n";
  cl::Platform default_platform = all_platforms[1];
  std::cout << "Using platform: "
            << default_platform.getInfo<CL_PLATFORM_NAME>() << "\n";

  std::vector<cl::Device> all_devices;
  default_platform.getDevices(CL_DEVICE_TYPE_GPU, &all_devices);
  for (unsigned i = 0; i < all_devices.size(); i++)
    std::cout << ' ' << all_devices.at(i).getInfo<CL_DEVICE_NAME>()
              << " with extension:"
              << all_devices.at(i).getInfo<CL_DEVICE_EXTENSIONS>() << "\n\n";
  if (all_devices.size() == 0) {
    std::cout << " No devices found. Check OpenCL installation!\n";
    exit(1);
  }
}
