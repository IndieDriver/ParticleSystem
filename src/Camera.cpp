#include "Camera.h"

Camera::Camera(Vec3 position, Vec3 targetPosition, int w, int h)
{
    pos = position;

    width = w;
    height = h;

	(void)targetPosition;
    mouseXpos = ((float)w / 2.0f);
    mouseYpos = ((float)h / 2.0f);

    verAngle = 0.0f;
    horAngle = 3.14f;
    update();
}

void Camera::update(){
    queryInput();
    double currentTime = glfwGetTime();
    deltaTime = float(currentTime - lastTime);
    lastTime = (float)currentTime;
    if (mouseMoved) {
        horAngle += 0.5f * deltaTime * (width / 2.0f - mouseXpos);
        verAngle += 0.5f * deltaTime * (height / 2.0f - mouseYpos);
        mouseMoved = false;
    }
    dir = Vec3(cos(verAngle) * sin(horAngle),
               sin(verAngle),
               cos(verAngle) * cos(horAngle));
    right = Vec3(sin(horAngle - 3.14f/2.0f),
                 0,
                 cos(horAngle - 3.14f/2.0f));
    up = right.cross(dir);
    view = viewMatrix(pos,
                      dir + pos,
                      up);
    proj = projMatrix(45.0f, ((float)WIDTH /(float)HEIGHT));
}

Matrix getMVP(Matrix model, Matrix view, Matrix proj){
    return (model * view * proj);
}

void Camera::queryInput() {
	if (inputHandler == nullptr)
        return;
	if (inputHandler->keybrDisabled)
        return;
    if (inputHandler->keys[GLFW_KEY_UP] || inputHandler->keys[GLFW_KEY_W]){
        Vec3 tmp = dir * speed * deltaTime;
        pos = pos + tmp;
    }
    if (inputHandler->keys[GLFW_KEY_DOWN] || inputHandler->keys[GLFW_KEY_S]){
        Vec3 tmp = dir * speed * deltaTime;
        pos = pos - tmp;
    }
    if (inputHandler->keys[GLFW_KEY_RIGHT] || inputHandler->keys[GLFW_KEY_D]){
        Vec3 right = up.cross(dir);
        Vec3 tmp = right * speed * deltaTime;
        pos = pos - tmp;
    }
    if (inputHandler->keys[GLFW_KEY_LEFT] || inputHandler->keys[GLFW_KEY_A]){
        Vec3 right = up.cross(dir);
        Vec3 tmp = right * speed * deltaTime;
        pos = pos + tmp;
    }
	if (inputHandler->mouseDisabled)
        return;
	if (inputHandler->mousex != mouseXpos || inputHandler->mousey != mouseYpos) {
		mouseXpos = inputHandler->mousex;
		mouseYpos = inputHandler->mousey;
		mouseMoved = true;
	}

}
