#include "camera.hpp"
#include "env.hpp"
#include "part.hpp"
#include "scene.hpp"
#include "shader.hpp"

int main(int ac, char **av) {
  unsigned int particle_count = 0;
  Env env(WIDTH, HEIGHT);
  GL_DUMP_ERROR("main");
  if (ac == 2) {
    try {
      particle_count = std::stoi(av[1]);
    } catch (std::exception &e) {
      std::cout << "[Exception] " << e.what() << std::endl;
    }
  } else if (ac == 1) {
    particle_count = PARTICLE_NUM;
  } else {
    std::cout << "Usage: ./ParticleSystem [num_particle]" << std::endl;
  }
  if (particle_count < 1 || particle_count > 10000000) {
    particle_count = PARTICLE_NUM;
    std::cout << "[Warning] Invalid number of particle, using default number"
              << std::endl;
  }
  Shader shader("shaders/part.frag", "shaders/part.vert");

  CLenv cl("shaders/kernel.cl");
  cl.createBuffer(particle_count);

  Camera camera(glm::vec3(0.0f, 0.0f, 10.0f), glm::vec3(0.0f, 0.0f, 0.0f),
                WIDTH, HEIGHT);

  Scene scene(&cl, &camera, particle_count);
  GL_DUMP_ERROR("scene");

  while (!glfwWindowShouldClose(env.window)) {
    glfwPollEvents();
    env.update();
    camera.update(env);
    scene.update(env);
    scene.draw(shader);
    glfwSwapBuffers(env.window);
    if (env.inputHandler.keys[GLFW_KEY_ESCAPE]) {
      glfwSetWindowShouldClose(env.window, GLFW_TRUE);
    }
  }
  glfwTerminate();
}
