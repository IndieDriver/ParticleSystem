#ifndef PARTICLESYSTEM_SCENE_H
#define PARTICLESYSTEM_SCENE_H

#include "part.h"
#include "env.h"
#include "CLenv.h"
#include "InputHandler.h"
#include "Camera.h"
#include "Shader.h"

class Scene {
public:
	Env             *env;
	CLenv           *cLenv;
	Camera          *camera;
	GLuint          vao;
	Scene(CLenv *env, Camera *camera);
	void            draw(const Shader &shader);
};


#endif //PARTICLESYSTEM_SCENE_H
