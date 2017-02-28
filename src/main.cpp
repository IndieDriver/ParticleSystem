#include "part.h"
#include "Shader.h"
#include "CLenv.h"
#include "Env.h"
#include "Camera.h"
#include "Scene.h"

void animate(bool init, CLenv& cl, Camera &camera, cl_float4 pos){
    try{

        //if (cur) {
            float x = 2.0f * camera.inputHandler->mousex / WIDTH - 1;
            float y = -2.0f * camera.inputHandler->mousey / HEIGHT + 1;
            Matrix viewProjectionInverse = get_inverse(mat4_mul(camera.view, camera.proj));
            Vec4 point3D = Vec4(x, y, 0, 0);
            Vec4 cursor = viewProjectionInverse.mul_matrix4_vec4(point3D);
            pos.x = cursor.x;
            pos.y = cursor.y;
            pos.z = 0.0f;
            pos.w = 0.0f;

            //pos.x = -1.0f;
            //pos.y = -1.0f;
            pos.z = 0.0f;
            pos.w = 0.0f;

        //printf("%f %f %f %d\n", pos.x, pos.y, pos.z, init);
        glFlush();
        glFinish();
        int status = 0;
        std::vector<cl::Memory> cl_vbos;
        cl_vbos.push_back(cl.buf_pos);
        cl_vbos.push_back(cl.buf_col);
        status = cl.cmds.enqueueAcquireGLObjects(&cl_vbos, NULL, NULL);
        if (init)
            cl.enqueueKernel(cl.kinit, pos);
        else
            cl.enqueueKernel(cl.kernel, pos);
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
    Camera camera(Vec3(0.0f, 0.0f, 5.0f), Vec3(0.0f, 0.0f, 0.0f), WIDTH, HEIGHT);
	camera.inputHandler = &inputHandler;
    glfwSetWindowUserPointer(env.window, &inputHandler);
    glfwSetInputMode(env.window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
    //glfwSetInputMode(env.window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    glfwSetCursorPosCallback(env.window, mouseCallback);
    glfwSetKeyCallback(env.window, keyCallback);
	//glfwSetCursorPos(env.window, (camera.width / 2.0), (camera.height / 2.0));

    Scene scene(&cl, &camera);

    bool anim = false;
	cl_float4 cursorpos;
    cursorpos.x = 0.0f;
    cursorpos.y = 0.0f;
    cursorpos.z = 0.0f;
    cursorpos.w = 0.0f;

    while (!glfwWindowShouldClose(env.window))
    {
        if (!anim) {
            //TODO: split animate to handle init
            animate(anim, cl, camera, cursorpos);
            anim = true;
        }else
            animate(anim, cl, camera, cursorpos);

        scene.draw(shader);

        env.updateFpsCounter();
        glfwSwapBuffers(env.window);
        glfwPollEvents();
	    camera.update();
        if (glfwGetKey (env.window, GLFW_KEY_SPACE)) {
            anim = true;
        }
        if (glfwGetKey (env.window, GLFW_KEY_ESCAPE)) {
            glfwSetWindowShouldClose(env.window, 1);
        }
    }
    glfwTerminate();
}