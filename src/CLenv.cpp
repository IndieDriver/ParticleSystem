#include "CLenv.h"

void CLenv::enqueueKernel(cl::Kernel k, cl_float4 cursorpos){
    try{
        k.setArg(0, sizeof(cl_float4), &cursorpos);
        k.setArg(1, sizeof(cl_mem), &buf_pos);
        k.setArg(2, sizeof(cl_mem), &buf_col);
        k.setArg(3, sizeof(cl_mem), &buf_vel);
        cmds.enqueueNDRangeKernel(k, 0, cl::NDRange(PARTICLE_NUM));
    }
    catch (cl::Error e)
    {
        std::cout << std::endl << e.what() << " : Error " << e.err() << std::endl;
    }
}

void CLenv::createBuffer() {
    try {
        glGenBuffers(1, &pos_id);
        glBindBuffer(GL_ARRAY_BUFFER, pos_id);
        glBufferData(GL_ARRAY_BUFFER, 4 * PARTICLE_NUM * sizeof(float), NULL, GL_STATIC_DRAW);

        glGenBuffers(1, &col_id);
        glBindBuffer(GL_ARRAY_BUFFER, col_id);
        glBufferData(GL_ARRAY_BUFFER, 4 * PARTICLE_NUM * sizeof(float), NULL, GL_STATIC_DRAW);

        glBindBuffer(GL_ARRAY_BUFFER, 0);
        int status = 0;

        buf_vel = cl::Buffer(this->context, CL_MEM_READ_WRITE | CL_MEM_ALLOC_HOST_PTR, 4 * sizeof(float) * PARTICLE_NUM, NULL, &status);

        if (status < 0)
            std::cout << status << "buffer error\n";
        //status = cmds.enqueueWriteBuffer(buf_vel, CL_TRUE, 0, 4 * sizeof(float) * PARTICLE_NUM, NULL);
        if (status < 0)
           std::cout << status << "enqueue error\n";
        buf_pos = cl::BufferGL(this->context, CL_MEM_READ_WRITE, pos_id);
        buf_col = cl::BufferGL(this->context, CL_MEM_READ_WRITE, col_id);

    }catch (cl::Error e) {
        std::cout << std::endl << e.what() << " : Error " << e.err() << std::endl;
    }
}

CLenv::CLenv(std::string kernelFileName)
{
    try {
        std::vector<cl::Platform> all_platforms;
        cl::Platform::get(&all_platforms);
        if (all_platforms.size() == 0) {
            std::cout << " No platforms found. Check OpenCL installation!\n";
            exit(1);
        }
        cl::Platform default_platform = all_platforms[1];
        std::cout << "Using platform: " << default_platform.getInfo<CL_PLATFORM_NAME>() << "\n";
        std::vector<cl::Device> all_devices;
        default_platform.getDevices(CL_DEVICE_TYPE_GPU, &all_devices);
        const cl::Device default_device = all_devices[0];
        #ifdef linux
        cl_context_properties properties[] = {
                    CL_GL_CONTEXT_KHR, (cl_context_properties) glXGetCurrentContext(),
                    CL_GLX_DISPLAY_KHR, (cl_context_properties) glXGetCurrentDisplay(),
                    CL_CONTEXT_PLATFORM, (cl_context_properties) (default_platform)(), 0};
        #elif defined _WIN32
        cl_context_properties properties[] = {
                CL_GL_CONTEXT_KHR, (cl_context_properties) wglGetCurrentContext(),
                CL_WGL_HDC_KHR, (cl_context_properties) wglGetCurrentDC(),
                CL_CONTEXT_PLATFORM, (cl_context_properties) (default_platform)(), 0};
		#elif defined __APPLE__
		CGLContextObj glContext = (CGLContextObj)CGLGetCurrentContext();
        //CGLContextObj glContext = (CGLContextObj) glfwGetCurrentContext();
		CGLShareGroupObj shareGroup = CGLGetShareGroup(glContext);
        cl_context_properties properties[] = {
                CL_CONTEXT_PROPERTY_USE_CGL_SHAREGROUP_APPLE,
                (cl_context_properties)shareGroup, 0};
        #endif

        context =  cl::Context(default_device, properties);
        cmds = cl::CommandQueue(context, default_device);
	    if (loadProgram(kernelFileName, default_device)){
            kinit = cl::Kernel(program, "clinit");
            kernel = cl::Kernel(program, "clpart");
        }
    }
    catch (cl::Error e)
    {
        std::cout << std::endl << e.what() << " : " << e.err() << std::endl;
    }
}


bool CLenv::loadProgram(std::string filename, cl::Device device){
    std::string line;
    std::ifstream file(filename);
    std::string buffer = "";

    if (file){
        while (getline(file, line))
            buffer += line + "\n";
        file.close();
    }
    cl::Program::Sources source(1, std::make_pair(buffer.c_str(), buffer.length()));
    program = cl::Program(context, source);
    try {
        program.build({device});
    } catch (cl::Error e){
        //cl_build_status status = program.getBuildInfo<CL_PROGRAM_BUILD_STATUS>(device);
        std::string name = device.getInfo<CL_DEVICE_NAME>();
        std::string buildlog = program.getBuildInfo<CL_PROGRAM_BUILD_LOG>(device);
        std::cerr << "Build log for " << name << ":" << std::endl << buildlog << std::endl;
        return (false);
    }
    return (true);
}

void debugPrintAllDevice(){
    std::vector<cl::Platform> all_platforms;
    cl::Platform::get(&all_platforms);
    if (all_platforms.size() == 0) {
        std::cout << " No platforms found. Check OpenCL installation!\n";
        exit(1);
    }
    for (unsigned i=0; i<all_platforms.size(); i++)
        std::cout << ' ' << all_platforms.at(i).getInfo<CL_PLATFORM_NAME>() << " with extension:" <<
                  all_platforms.at(i).getInfo<CL_PLATFORM_EXTENSIONS>()<<"\n\n";
    cl::Platform default_platform = all_platforms[1];
    std::cout << "Using platform: " << default_platform.getInfo<CL_PLATFORM_NAME>() << "\n";

    std::vector<cl::Device> all_devices;
    default_platform.getDevices(CL_DEVICE_TYPE_GPU, &all_devices);
    for (unsigned i=0; i<all_devices.size(); i++)
        std::cout << ' ' << all_devices.at(i).getInfo<CL_DEVICE_NAME>() << " with extension:" <<
                  all_devices.at(i).getInfo<CL_DEVICE_EXTENSIONS>()<< "\n\n";
    if (all_devices.size() == 0) {
        std::cout << " No devices found. Check OpenCL installation!\n";
        exit(1);
    }
}

