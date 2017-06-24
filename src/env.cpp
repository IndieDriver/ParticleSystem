#include "env.h"

Env::Env(int width, int height) {
    if (!glfwInit())
        return ;
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 1);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    window = glfwCreateWindow(width, height, "Particle System", NULL, NULL);
    if (!window)
    {
        std::cout << "Could not create window\n";
        glfwTerminate();
        return ;
    }
    glfwMakeContextCurrent(window);
    if(!gladLoadGL()) {
		std::cout << "Glad loading error " << std::endl;
        exit(-1);
    }
	std::cout << "OpenGL " << glGetString(GL_VERSION) << std::endl;
	std::cout << "GLSL " << glGetString(GL_SHADING_LANGUAGE_VERSION) << std::endl;
    return ;
}

void Env::updateFpsCounter() {
    static double previous_seconds = glfwGetTime ();
    static int frame_count;
    double current_seconds = glfwGetTime ();
    double elapsed_seconds = current_seconds - previous_seconds;
    if (elapsed_seconds > 0.25) {
        previous_seconds = current_seconds;
        double fps = (double)frame_count / elapsed_seconds;
        char tmp[128];
        sprintf (tmp, "opengl @ fps: %.2f", fps);
        glfwSetWindowTitle (window, tmp);
        frame_count = 0;
    }
    frame_count++;
}
