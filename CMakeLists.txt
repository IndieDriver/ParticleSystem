cmake_minimum_required(VERSION 3.5)
project(ParticleSystem)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=gnu++11 -g")
find_package(OpenGL REQUIRED)

#Handle GLFW
set(GLFW_BUILD_DOCS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_TESTS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)

add_subdirectory(third-party/glfw)
include_directories(third-party/glfw/include)
include_directories(third-party/glad)
include_directories(third-party)

SET(CMAKE_MODULE_PATH ${PROJECT_SOURCE_DIR}/cmake)

FIND_PACKAGE(OpenCL REQUIRED)

#INCLUDE_DIRECTORIES($ENV{AMDAPPSDKROOT}/include)
INCLUDE_DIRECTORIES(${OPENCL_INCLUDE_DIR})

set(SOURCE_FILES
        third-party/glad/glad.c
        src/main.cpp src/part.h
        src/env.cpp src/env.h
        src/CLenv.cpp src/CLenv.h
        src/Shader.cpp src/Shader.h
        src/Matrix.cpp src/Matrix.h
        src/Vec3.cpp src/Vec3.h
        src/Camera.cpp src/Camera.h
        src/InputHandler.cpp src/InputHandler.h src/Scene.h src/Scene.cpp)
add_executable(ParticleSystem ${SOURCE_FILES})

#Link GLFW and OpenCL
#target_link_libraries(ParticleSystem ${GLEW_LIBRARY})
target_link_libraries(ParticleSystem glfw ${GLFW_LIBRARIES})
target_link_libraries(ParticleSystem ${OPENCL_LIBRARIES})

