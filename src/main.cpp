#include "part.h"
#include "Shader.h"
#include "CLenv.h"
#include "env.h"
#include "Camera.h"
#include "Scene.h"

int main(void)
{
    Env env(WIDTH, HEIGHT);
    Shader shader("shaders/part.fs.glsl", "shaders/part.vs.glsl");
    glEnable(GL_DEBUG_OUTPUT);
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LESS);

    CLenv cl("shaders/kernel.cl");
    cl.createBuffer();

    InputHandler inputHandler;
    Camera camera(Vec3(0.0f, 0.0f, 10.0f), Vec3(0.0f, 0.0f, 0.0f), WIDTH, HEIGHT);
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
    scene.initScene();
    while (!glfwWindowShouldClose(env.window)) {
	    scene.queryInput(env);
        scene.draw(shader);
	    if (scene.needInit)
            scene.initScene();
        else
            scene.animate(env.getDeltaTime());
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
