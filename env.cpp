//
// Created by amath on 9/27/2016.
//

#include "env.h"

void Env::init_windows() {

    if (!glfwInit())
        return ;
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 1);
    //glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    Env::window = glfwCreateWindow(1280, 720, "Particle System", NULL, NULL);
    if (!Env::window)
    {
        std::cout << "Could not create window\n";
        glfwTerminate();
        return ;
    }
    glfwMakeContextCurrent(Env::window);
    if(!gladLoadGL()) {
        printf("Something went wrong!\n");
        exit(-1);
    }
    printf("OpenGL %d.%d\n", GLVersion.major, GLVersion.minor);
    if (GLVersion.major < 2) {
        printf("Your system doesn't support OpenGL >= 2!\n");
        exit(0);
    }

    printf("OpenGL %s, GLSL %s\n", glGetString(GL_VERSION),
           glGetString(GL_SHADING_LANGUAGE_VERSION));

    return ;
}
