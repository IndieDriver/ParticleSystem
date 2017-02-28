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
		cl_float4 pos;
		pos.x = 0.0f;
		pos.y = 0.0f;
		pos.z = 0.0f;
		pos.w = 0.0f;

		glFlush();
		glFinish();
		int status = 0;
		std::vector<cl::Memory> cl_vbos;
		cl_vbos.push_back(cl->buf_pos);
		cl_vbos.push_back(cl->buf_col);
		cl->cmds.enqueueAcquireGLObjects(&cl_vbos, NULL, NULL);
		cl->enqueueKernel(cl->kinit, pos);
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
	try{
		//if (cur) {
		float x = 2.0f * camera->inputHandler->mousex / WIDTH - 1;
		float y = -2.0f * camera->inputHandler->mousey / HEIGHT + 1;
		Matrix viewProjectionInverse = get_inverse(mat4_mul(camera->view, camera->proj));
		Vec4 point3D = Vec4(x, y, 0, 0);
		Vec4 cursor = viewProjectionInverse.mul_matrix4_vec4(point3D);


		cursorPos.x = cursor.x;
		cursorPos.y = cursor.y;
		cursorPos.z = 0.0f;
		//cursorPos.z = cursor.z;
		//cursorPos.w = cursor.w;
		cursorPos = getCursorPosInWorldSpace();
		cursorPos.z =0.0f;

		//printf("%f %f %f %d\n", pos.x, pos.y, pos.z, init);
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
	}
	catch (cl::Error e)
	{
		std::cout << std::endl << e.what() << " : Error " << e.err() << std::endl;
	}
}

cl_float4 Scene::getCursorPosInWorldSpace() {
	cl_float4 result;
	float screenSpaceX = (camera->inputHandler->mousex / ((float)WIDTH / 2.0f) - 1.0f) * (float)WIDTH / (float)HEIGHT;
	float screenSpaceY = 1.0f - camera->inputHandler->mousey / ((float)HEIGHT / 2.0f);

	Vec3 dx = camera->right * screenSpaceX;
	Vec3 dy = camera->up * screenSpaceY;

	Vec3 dir = camera->dir + (dx + dy) * 2.0f;
	dir.normalize();

	Vec3 planNormal = neg_vec(camera->dir);
	planNormal.normalize();
	Vec3 planPos = Vec3(0.0f, 0.0f, 0.0f);

	float t;


	t = -(((planNormal.x * (camera->pos.x - planPos.x)) +
	       (planNormal.y * (camera->pos.y - planPos.y)) +
	       (planNormal.z * (camera->pos.z - planPos.z)))
	      / ((planNormal.x * dir.x) + (planNormal.y * dir.y)
	         + (planNormal.z * dir.z)));
	if (t < 0.0)
		printf("Sould not happened\n");

	result.x = camera->pos.x + dir.x * t;
	result.y = camera->pos.y + dir.y * t;
	result.z = camera->pos.z + dir.z * t;
	return (result);
}
