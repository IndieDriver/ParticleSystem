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
	CLenv           *cl;
	Camera          *camera;
	GLuint          vao;
	bool            needInit = true;
	bool            isFreeCam = false;
	bool            gravity = false;
	bool            sphere = true;
	bool			shouldUpdateCursorPos = true;
	cl_float4 		lastCursorPos;
	Scene(CLenv *env, Camera *camera);
	void            draw(const Shader &shader, int num_particle);
	void            initScene(int num_particle);
	void            animate(int num_particle, float deltaTime);
	void            queryInput(Env &env);
	cl_float4       getCursorPosInWorldSpace();
};

#endif //PARTICLESYSTEM_SCENE_H
