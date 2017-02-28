#ifndef PARTICLESYSTEM_CAMERA_H
#define PARTICLESYSTEM_CAMERA_H

#include "part.h"
#include "InputHandler.h"

class Camera {
public:
	Vec3    pos;
	Vec3    dir;
	Vec3    right;
	Vec3    up;
	Matrix  proj;
	Matrix  view;
	int     width;
	int     height;
	InputHandler *inputHandler = nullptr;

	Camera(Vec3 pos = Vec3(), Vec3 rot = Vec3(), int width = 0, int height = 0);
	void queryInput();
	void update();
private:
	//Vec3 target;
	bool  mouseMoved = false;
	float mouseXpos;
	float mouseYpos;
	float horAngle;
	float verAngle;
	float speed = 3.0f;
	float deltaTime;
	float lastTime;
};


#endif //PARTICLESYSTEM_CAMERA_H
