cmake_minimum_required(VERSION 3.6)
project(ParticleSystem)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=gnu++11")
find_package(OpenGL REQUIRED)

#find_package(GLEW REQUIRED)
#if (GLEW_FOUND)
#   include_directories(${GLEW_INCLUDE_DIRS})
#    link_libraries(${GLEW_LIBRARIES})
#endif()

#Handle GLFW
add_subdirectory(${CMAKE_CURRENT_LIST_DIR}/glfw)
include_directories(glfw/include)
include_directories(glad)

SET(CMAKE_MODULE_PATH ${PROJECT_SOURCE_DIR}/cmake)

FIND_PACKAGE(OpenCL)

message(ERROR "OpenCL found: ${OPENCL_FOUND}")
message(ERROR "OpenCL includes: ${OPENCL_INCLUDE_DIRS}")
message(ERROR "OpenCL CXX includes: ${OPENCL_HAS_CPP_BINDINGS}")
message(ERROR "OpenCL libraries: ${OPENCL_LIBRARIES}")

INCLUDE_DIRECTORIES($ENV{AMDAPPSDKROOT}/include)
#INCLUDE_DIRECTORIES(${OPENCL_INCLUDE_DIR})

set(SOURCE_FILES glad/glad.c main.cpp env.cpp env.h part.h CLenv.cpp CLenv.h GLenv.cpp GLenv.h Matrix.cpp Matrix.h Vec4.cpp Vec4.h Camera.cpp Camera.h)
add_executable(ParticleSystem ${SOURCE_FILES})

#Link GLFW and OpenCL
#target_link_libraries(ParticleSystem ${GLEW_LIBRARY})
target_link_libraries(ParticleSystem glfw ${GLFW_LIBRARIES})
target_link_libraries(ParticleSystem ${OPENCL_LIBRARIES})

