#include "Scene.h"

Scene::Scene(CLenv *clenv, Camera *cam) {
	cl = clenv;
	camera = cam;
	vao = 0;
	glGenVertexArrays (1, &vao);
	glBindVertexArray (vao);

	glBindBuffer(GL_ARRAY_BUFFER, cl->pos_id);
	glVertexAttribPointer (0, 4, GL_FLOAT, GL_FALSE, 0, NULL);

	glBindBuffer(GL_ARRAY_BUFFER, cl->col_id);
	glVertexAttribPointer (1, 4, GL_FLOAT, GL_FALSE, 0, NULL);

	glEnableVertexAttribArray (0);
	glEnableVertexAttribArray (1);
}

void Scene::draw(const Shader &shader) {
	GLint mvpID = glGetUniformLocation(shader.id, "MVP");
	Matrix model = modelMatrix(Vec3(0.0f), Vec3(0.0f), Vec3(1.0f, 1.0f, 1.0f));
	Matrix MVP = getMVP(model, camera->view, camera->proj);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	shader.use();
	if (glGetError() != GL_NO_ERROR)
		std::cout << "Error\n";
	glUniformMatrix4fv(mvpID, 1, GL_FALSE, MVP.mat4);
	glBindVertexArray (vao);
	glDrawArrays (GL_POINTS, 0, PARTICLE_NUM);
}

void Scene::initScene(){
	//TODO: init in cube
	try{
		cl_float4 cursorPos;
		cursorPos.x = static_cast <float> (rand()) / static_cast <float> (RAND_MAX);
		cursorPos.y = static_cast <float> (rand()) / static_cast <float> (RAND_MAX);
		cursorPos.z = static_cast <float> (rand()) / static_cast <float> (RAND_MAX);
		cursorPos.w = static_cast <float> (rand()) / static_cast <float> (RAND_MAX);

		glFlush();
		glFinish();
		int status = 0;
		std::vector<cl::Memory> cl_vbos;
		cl_vbos.push_back(cl->buf_pos);
		cl_vbos.push_back(cl->buf_col);
		cl->cmds.enqueueAcquireGLObjects(&cl_vbos, NULL, NULL);
		cl->enqueueKernel(cl->kinit, cursorPos);
		status = cl->cmds.finish();
		if (status < 0)
			printf("Error clfinish\n");
		cl->cmds.enqueueReleaseGLObjects(&cl_vbos, NULL, NULL);
	}
	catch (cl::Error e)
	{
		std::cout << std::endl << e.what() << " : Error " << e.err() << std::endl;
	}

}

void Scene::animate(cl_float4 cursorPos){
	if (gravity)
		cursorPos = getCursorPosInWorldSpace();
	else {
		cursorPos.x = -1.0f;
		cursorPos.y = -1.0f;
		cursorPos.z = -1.0f;
		cursorPos.w = -1.0f;
	}
	try{
		glFlush();
		glFinish();
		int status = 0;
		std::vector<cl::Memory> cl_vbos;
		cl_vbos.push_back(cl->buf_pos);
		cl_vbos.push_back(cl->buf_col);
		status = cl->cmds.enqueueAcquireGLObjects(&cl_vbos, NULL, NULL);
		cl->enqueueKernel(cl->kernel, cursorPos);
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

	Matrix  invProj = get_inverse(camera->proj);
	Vec4    rayEye = invProj.mul_matrix4_vec4(rayClip);
	rayEye = Vec4(rayEye.x, rayEye.y, -1.0f, 0.0f);

	Matrix  invView = get_inverse(camera->view);
	Vec4    rayWorld4 = invView.mul_matrix4_vec4(rayEye);
	Vec3    rayWorld(rayWorld4.x, rayWorld4.y, rayWorld4.z);
	rayWorld.normalize();

	Vec3 planNormal = camera->dir;
	planNormal.normalize();
	Vec3 planPos = Vec3(0.0f, 0.0f, 0.0f);

	float t;
	t = -(((planNormal.x * (camera->pos.x - planPos.x)) +
	       (planNormal.y * (camera->pos.y - planPos.y)) +
	       (planNormal.z * (camera->pos.z - planPos.z)))
	      / ((planNormal.x * rayWorld.x) + (planNormal.y * rayWorld.y)
	         + (planNormal.z * rayWorld.z)));
	if (t < 0.0)
		printf("Sould not happened\n");

	cl_float4 result;
	result.x = camera->pos.x + rayWorld.x * t;
	result.y = camera->pos.y + rayWorld.y * t;
	result.z = camera->pos.z + rayWorld.z * t;

	return (result);
}

void Scene::queryInput() {
	if (camera->inputHandler == nullptr)
		return;
	if (camera->inputHandler->keys[GLFW_KEY_SPACE]){
		gravity = true;
	}
	if (camera->inputHandler->keys[GLFW_KEY_F]) {
		camera->inputHandler->keybrDisabled = !camera->inputHandler->keybrDisabled;
		//camera->inputHandler->mouseDisabled = !camera->inputHandler->mouseDisabled;
		//gravity = false;
	}
}
