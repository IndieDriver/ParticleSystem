#include "Scene.h"

Scene::Scene(CLenv *clenv, Camera *cam) {
	cl = clenv;
	camera = cam;
	vao = 0;
	glGenVertexArrays (1, &vao);
	glBindVertexArray (vao);

	glBindBuffer(GL_ARRAY_BUFFER, cl->pos_id);
	glVertexAttribPointer (0, 4, GL_FLOAT, GL_FALSE, 0, NULL);

	glEnableVertexAttribArray (0);
}

void Scene::draw(const Shader &shader, int num_particle) {
	GLint mvpID = glGetUniformLocation(shader.id, "MVP");
	GLint cursorPosID = glGetUniformLocation(shader.id, "cursorPos");
	Matrix model = modelMatrix(Vec3(0.0f), Vec3(0.0f), Vec3(1.0f, 1.0f, 1.0f));
	Matrix MVP = getMVP(model, camera->view, camera->proj);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	shader.use();
	glUniformMatrix4fv(glGetUniformLocation(shader.id, "MVP"), 1, GL_FALSE,
			MVP.mat4);
	glUniform3fv(glGetUniformLocation(shader.id, "cursorPos"), 1,
			&(lastCursorPos.x));

	glBindVertexArray (vao);
	glDrawArrays (GL_POINTS, 0, num_particle);
}

void Scene::initScene(int num_particle){
	try{
		if (shouldUpdateCursorPos) {
			lastCursorPos = getCursorPosInWorldSpace();
		}
		if (sphere) {
			lastCursorPos.w = -1.0f;
		} else {
			lastCursorPos.w = 0.0f;
		}
		needInit = false;
		glFlush();
		glFinish();
		int status = 0;
		std::vector<cl::Memory> cl_vbos;
		cl_vbos.push_back(cl->buf_pos);
		cl->cmds.enqueueAcquireGLObjects(&cl_vbos, NULL, NULL);
		cl->enqueueKernel(cl->kinit, lastCursorPos, num_particle, 0.0f);
		status = cl->cmds.finish();
		if (status < 0)
			printf("Error clfinish\n");
		cl->cmds.enqueueReleaseGLObjects(&cl_vbos, NULL, NULL);
	}
	catch (cl::Error e) {
		std::cout << std::endl << e.what() << " : Error " << e.err() << std::endl;
	}
}

void Scene::animate(int num_particle, float deltaTime){
	if (shouldUpdateCursorPos)
		lastCursorPos = getCursorPosInWorldSpace();
	if (gravity)
		lastCursorPos.w = 0.0f;
	else
		lastCursorPos.w = -1.0f;
	try{
		glFlush();
		glFinish();
		int status = 0;
		std::vector<cl::Memory> cl_vbos;
		cl_vbos.push_back(cl->buf_pos);
		status = cl->cmds.enqueueAcquireGLObjects(&cl_vbos, NULL, NULL);
		cl->enqueueKernel(cl->kernel, lastCursorPos, num_particle, deltaTime);
		status = cl->cmds.finish();
		if (status < 0)
			printf("Error clfinish\n");
		cl->cmds.enqueueReleaseGLObjects(&cl_vbos, NULL, NULL);
	} catch (cl::Error e) {
		std::cout << std::endl << e.what() << " : Error " << e.err() << std::endl;
	}
}

cl_float4 Scene::getCursorPosInWorldSpace() {
	float   x = (2.0f * camera->inputHandler->mousex) / WIDTH - 1.0f;
	float   y = 1.0f - (2.0f * camera->inputHandler->mousey) / HEIGHT;
	Vec4    rayClip(x, y, -1.0f, 1.0f);

	Matrix  invProj = inverse(camera->proj);
	Vec4    rayEye = invProj * rayClip;
	rayEye = Vec4(rayEye.x, rayEye.y, -1.0f, 0.0f);
	Matrix  invView = inverse(camera->view);

	Vec4    rayWorld4 = invView * rayEye;

	Vec3    rayWorld(rayWorld4);
	rayWorld.normalize();

	Vec3 planNormal = camera->dir;
	planNormal.normalize();

	Vec3 planPos = camera->pos + normalize(rayWorld) * 10.0f;

	float denom = planNormal.dot(rayWorld);
	if (fabs(denom) > 1e-6) {
		float t = (planPos - camera->pos).dot(rayWorld) / denom;
		if (t >= 1e-6) {
			cl_float4 result;
			result.s[0] = camera->pos.x + rayWorld.x * t;
			result.s[1] = camera->pos.y + rayWorld.y * t;
			result.s[2] = camera->pos.z + rayWorld.z * t;
			result.s[3] = 1.0f;
			return (result);
		} else {
			std::cout << "invalid t" << std::endl;
		}
	} else {
		std::cout << "invalid dot" << std::endl;
	}

	cl_float4 zero;
	zero.s[0] = 0.0f;
	zero.s[1] = 0.0f;
	zero.s[2] = 0.0f;
	zero.s[3] = 0.0f;

	return (zero);
}

void Scene::queryInput(Env &env) {
	if (camera->inputHandler == nullptr)
		return;
	if (camera->inputHandler->keys[GLFW_KEY_SPACE]){
		camera->inputHandler->keys[GLFW_KEY_SPACE] = false;
		gravity = !gravity;
	}
	if (camera->inputHandler->keys[GLFW_KEY_F]) {
		camera->inputHandler->keys[GLFW_KEY_F] = false;
		isFreeCam = !isFreeCam;
		if (isFreeCam) {
			camera->inputHandler->keybrDisabled = false;
			camera->inputHandler->mouseDisabled = false;
			camera->inputHandler->edgeDetector = true;
			shouldUpdateCursorPos = false;
			camera->mouseXpos = camera->inputHandler->mousex;
			camera->mouseYpos = camera->inputHandler->mousey;
    		glfwSetInputMode(env.window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
		} else {
			camera->inputHandler->keybrDisabled = true;
			camera->inputHandler->mouseDisabled = true;
			shouldUpdateCursorPos = false;
    		glfwSetInputMode(env.window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
		}
	}
	if (camera->inputHandler->keys[GLFW_KEY_J]) {
		camera->inputHandler->keys[GLFW_KEY_J] = false;
		sphere = !sphere;
		gravity = false;
		needInit = true;
	}
	if (camera->inputHandler->keys[GLFW_KEY_G]) {
		camera->inputHandler->keys[GLFW_KEY_G] = false;
		shouldUpdateCursorPos = !shouldUpdateCursorPos;
	}
}
