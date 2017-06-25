#ifndef PARTICLESYSTEM_INPUTHANDLER_H
#define PARTICLESYSTEM_INPUTHANDLER_H

#include "part.h"

class InputHandler {
public:
	bool    keys[1024] = {0};
	bool    mouseDisabled = true;
	bool    keybrDisabled = true;
	float   mousex = (float)(WIDTH / 2);
	float   mousey = (float)(HEIGHT / 2);
	bool	edgeDetector = false; //To smooth transition between mouse inputmode
};

void    keyCallback(GLFWwindow *window, int key, int scancode, int action, int mode);
void    mouseCallback(GLFWwindow *window, double xpos, double ypos);
#endif //PARTICLESYSTEM_INPUTHANDLER_H
