#include "Scene.h"


Scene::Scene(CLenv *clenv, Camera *cam) {
	cLenv = clenv;
	camera = cam;
	vao = 0;
	glGenVertexArrays (1, &vao);
	glBindVertexArray (vao);

	glBindBuffer(GL_ARRAY_BUFFER, cLenv->pos_id);
	glVertexAttribPointer (0, 4, GL_FLOAT, GL_FALSE, 0, NULL);

	glBindBuffer(GL_ARRAY_BUFFER, cLenv->col_id);
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
