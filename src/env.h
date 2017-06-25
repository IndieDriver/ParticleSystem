#ifndef PARTICLESYSTEM_ENV_H
#define PARTICLESYSTEM_ENV_H

#include "part.h"

class Env {
public:
	GLFWwindow* window;
	int particle_num;
	Env(int width, int height);

	void	updateFpsCounter();
	float	getDeltaTime();
};


#endif //PARTICLESYSTEM_ENV_H
