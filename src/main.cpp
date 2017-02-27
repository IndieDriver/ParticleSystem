#include "part.h"
#include "Shader.h"
#include "CLenv.h"
#include "Env.h"
#include "Camera.h"

static bool     cur = false;
static cl_float4 cursorpos;
static float    mposx;
static float    mposy;

static void cursor_position_callback(GLFWwindow* window, double xpos, double ypos)
{
        mposx = xpos;
        mposy = ypos;
}

void animate(CLenv& cl, Camera &camera, int init){
    try{
        if (cur) {
            float x = 2.0f * mposx / 1280 - 1;
            float y = -2.0f * mposy / 720 + 1;
            Matrix viewProjectionInverse = get_inverse(mat4_mul(camera.view, camera.proj));
            Vec4 point3D = Vec4(x, y, 0, 0);
            Vec4 cursor = viewProjectionInverse.mul_matrix4_vec4(point3D);
            cursorpos.x = cursor.x;
            cursorpos.y = cursor.y;
            cursorpos.z = 0.0f;
            cursorpos.w = 0.0f;
        } else
        {
            cursorpos.x = -1.0f;
            cursorpos.y = -1.0f;
            cursorpos.z = 0.0f;
            cursorpos.w = 0.0f;
        }

        glFlush();
        glFinish();
        int status = 0;
        std::vector<cl::Memory> cl_vbos;
        cl_vbos.push_back(cl.buf_pos);
        cl_vbos.push_back(cl.buf_col);
        status = cl.cmds.enqueueAcquireGLObjects(&cl_vbos, NULL, NULL);
        if (init == 1)
            cl.enqueueKernel(cl.kinit, cursorpos);
        else
            cl.enqueueKernel(cl.kernel, cursorpos);
        status = cl.cmds.finish();
        if (status < 0)
            printf("Error clfinish\n");
        cl.cmds.enqueueReleaseGLObjects(&cl_vbos, NULL, NULL);
    }
    catch (cl::Error e)
    {
        std::cout << std::endl << e.what() << " : Error " << e.err() << std::endl;
    }
}

int main(void)
{
    Env env(WIDTH, HEIGHT);

    Shader shader("shaders/part.fs.glsl", "shaders/part.vs.glsl");
    glEnable(GL_DEBUG_OUTPUT);
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LESS);
    CLenv cl("shaders/kernel.cl");
    cl.createBuffer();

    InputHandler inputHandler;
    Camera camera(Vec3(0.0f, 2.0f, 5.0f), Vec3(0.0f, 0.0f, 0.0f), 1280, 720);
	camera.inputHandler = &inputHandler;
    glfwSetWindowUserPointer(env.window, &inputHandler);

    GLuint vao = 0;
    glGenVertexArrays (1, &vao);
    glBindVertexArray (vao);

    glBindBuffer(GL_ARRAY_BUFFER, cl.pos_id);
    glVertexAttribPointer (0, 4, GL_FLOAT, GL_FALSE, 0, NULL);

    glBindBuffer(GL_ARRAY_BUFFER, cl.col_id);
    glVertexAttribPointer (1, 4, GL_FLOAT, GL_FALSE, 0, NULL);

    glEnableVertexAttribArray (0);
    glEnableVertexAttribArray (1);
    int init = 1;
    cursorpos.x = 0.0f;
    cursorpos.y = 0.0f;
    cursorpos.z = 0.0f;
    cursorpos.w = 0.0f;
    glfwSetInputMode(env.window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    glfwSetCursorPosCallback(env.window, mouseCallback);
    glfwSetKeyCallback(env.window, keyCallback);
	glfwSetCursorPos(env.window, (camera.width / 2.0), (camera.height / 2.0));
    while (!glfwWindowShouldClose(env.window))
    {
        if (init) {
            animate(cl, camera, 1);
            init = 0;
        }else
            animate(cl, camera, 0);

        GLint mvpID = glGetUniformLocation(shader.id, "MVP");
        Matrix model = modelMatrix(Vec3(0.0f), Vec3(0.0f), Vec3(0.05f, 0.05f, 0.05f));
        Matrix MVP = getMVP(model, camera.view, camera.proj);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        shader.use();
        if (glGetError() != GL_NO_ERROR)
            std::cout << "Error\n";
        glUniformMatrix4fv(mvpID, 1, GL_FALSE, MVP.mat4);
        glBindVertexArray (vao);
        glDrawArrays (GL_POINTS, 0, PARTICLE_NUM);

        env.updateFpsCounter();
        glfwSwapBuffers (env.window);
        glfwPollEvents ();
	    camera.update();
        if (glfwGetKey (env.window, GLFW_KEY_SPACE)) {
            cur = true;
        }
        if (glfwGetKey (env.window, GLFW_KEY_ESCAPE)) {
            glfwSetWindowShouldClose(env.window, 1);
        }
    }
    glfwTerminate();
}