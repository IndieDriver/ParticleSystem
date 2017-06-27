#include "part.h"
#include "Shader.h"
#include "CLenv.h"
#include "env.h"
#include "Camera.h"
#include "Scene.h"

int main(int ac, char **av) {
    Env env(WIDTH, HEIGHT);
	if (ac == 2) {
		try {
			env.particle_num = std::stoi(av[1]);
		} catch (std::exception &e) {
			std::cout << "[Exception] " << e.what() << std::endl;
		}
	} else if (ac == 1){
		env.particle_num = PARTICLE_NUM;
	} else {
		std::cout << "Usage: ./ParticleSystem [num_particle]" << std::endl;
	}
	if (env.particle_num < 1 || env.particle_num > 10000000) {
		env.particle_num = PARTICLE_NUM;
		std::cout << "[Warning] Invalid number of particle, using default number"
			<< std::endl;
	}
    Shader shader("shaders/part.fs.glsl", "shaders/part.vs.glsl");
    glEnable(GL_DEBUG_OUTPUT);
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LESS);

    CLenv cl("shaders/kernel.cl");
    cl.createBuffer(env.particle_num);

    InputHandler inputHandler;
    Camera camera(Vec3(0.0f, 0.0f, -10.0f), Vec3(0.0f, 0.0f, 0.0f), WIDTH, HEIGHT);
	camera.inputHandler = &inputHandler;
    glfwSetWindowUserPointer(env.window, &inputHandler);
    glfwSetInputMode(env.window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
	glfwSetCursorPos(env.window, (camera.width / 2.0), (camera.height / 2.0));
    glfwSetInputMode(env.window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
    glfwSetCursorPosCallback(env.window, mouseCallback);
    glfwSetKeyCallback(env.window, keyCallback);
	glfwSwapInterval(0);

    Scene scene(&cl, &camera);

    bool anim = false;
    scene.initScene(env.particle_num);
    while (!glfwWindowShouldClose(env.window)) {
	    scene.queryInput(env);
        scene.draw(shader, env.particle_num);
	    if (scene.needInit)
            scene.initScene(env.particle_num);
        else
            scene.animate(env.particle_num, env.getDeltaTime());
        env.updateFpsCounter();
        glfwSwapBuffers(env.window);
        glfwPollEvents();
	    camera.update();
        if (glfwGetKey (env.window, GLFW_KEY_ESCAPE)) {
            glfwSetWindowShouldClose(env.window, 1);
        }
    }
    glfwTerminate();
}
