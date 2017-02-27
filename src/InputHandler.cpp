#include "InputHandler.h"

void    keyCallback(GLFWwindow *window, int key, int scancode, int action, int mode) {
	InputHandler *inputHandler = reinterpret_cast<InputHandler *>(glfwGetWindowUserPointer(window));
    if (action == GLFW_PRESS){
	    inputHandler->keys[key] = true;
    } else if (action == GLFW_RELEASE){
	    inputHandler->keys[key] = false;
    }
}

void mouseCallback(GLFWwindow *window, double xpos, double ypos) {
	InputHandler *inputHandler = reinterpret_cast<InputHandler *>(glfwGetWindowUserPointer(window));
	inputHandler->mousex = (float)xpos;
	inputHandler->mousey = (float)ypos;
    glfwSetCursorPos(window, (WIDTH / 2.0),(HEIGHT / 2.0));
}