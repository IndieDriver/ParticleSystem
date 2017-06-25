#ifndef PARTICLESYSTEM_CLENV_H
#define PARTICLESYSTEM_CLENV_H
#include "part.h"

class CLenv {
    public:
        CLenv(std::string file_name);
        void createBuffer(int num_particle);
        void enqueueKernel(cl::Kernel kernel, cl_float4 cursor, int num_particle,
				float deltaTime);
        cl::Device	        id;
        cl::Context			context;
        cl::CommandQueue	cmds;
        cl::Program			program;
        cl::Kernel          kinit;
        cl::Kernel          kernel;
        GLuint              pos_id;
        GLuint              col_id;
        cl::Buffer          buf_vel;
        cl::BufferGL        buf_pos;
        cl::BufferGL        buf_col;

		bool    loadProgram(std::string filename, cl::Device device);
};


#endif //PARTICLESYSTEM_CLENV_H
