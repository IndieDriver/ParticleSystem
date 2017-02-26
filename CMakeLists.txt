cmake_minimum_required(VERSION 3.6)
project(ParticleSystem)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=gnu++11")
find_package(OpenGL REQUIRED)

#Handle GLFW
add_subdirectory(third-party/glfw)
include_directories(third-party/glfw/include)
include_directories(third-party/glad)

SET(CMAKE_MODULE_PATH ${PROJECT_SOURCE_DIR}/cmake)

FIND_PACKAGE(OpenCL)

message(ERROR "OpenCL found: ${OPENCL_FOUND}")
message(ERROR "OpenCL includes: ${OPENCL_INCLUDE_DIRS}")
message(ERROR "OpenCL CXX includes: ${OPENCL_HAS_CPP_BINDINGS}")
message(ERROR "OpenCL libraries: ${OPENCL_LIBRARIES}")

INCLUDE_DIRECTORIES($ENV{AMDAPPSDKROOT}/include)
#INCLUDE_DIRECTORIES(${OPENCL_INCLUDE_DIR})

set(SOURCE_FILES
        third-party/glad/glad.c
        src/main.cpp src/part.h
        src/env.cpp src/env.h
        src/CLenv.cpp src/CLenv.h
        src/GLenv.cpp src/GLenv.h
        src/Matrix.cpp src/Matrix.h
        src/Vec3.cpp src/Vec3.h
        src/Camera.cpp src/Camera.h)
add_executable(ParticleSystem ${SOURCE_FILES})

#Link GLFW and OpenCL
#target_link_libraries(ParticleSystem ${GLEW_LIBRARY})
target_link_libraries(ParticleSystem glfw ${GLFW_LIBRARIES})
target_link_libraries(ParticleSystem ${OPENCL_LIBRARIES})

