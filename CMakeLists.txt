cmake_minimum_required(VERSION 3.5)
project(ParticleSystem)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_BUILD_TYPE RelWithDebInfo)
SET(CMAKE_MODULE_PATH ${PROJECT_SOURCE_DIR}/cmake)

find_package(OpenGL REQUIRED)

set(GLFW_BUILD_DOCS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_TESTS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)

if (MSVC)
  set(USE_MSVC_RUNTIME_LIBRARY_DLL ON)
endif(MSVC)

add_subdirectory(third-party/glfw)
include_directories(third-party/glfw/include)
include_directories(third-party/glad)
include_directories(third-party/glm)
include_directories(third-party/stb)
include_directories(third-party)

SET(CMAKE_MODULE_PATH ${PROJECT_SOURCE_DIR}/cmake)

FIND_PACKAGE(OpenCL REQUIRED)

INCLUDE_DIRECTORIES(${OPENCL_INCLUDE_DIR})

set(SOURCE_FILES
        third-party/glad/glad.c
        src/main.cpp
        src/env.cpp
        src/shader.cpp
        src/camera.cpp
        src/scene.cpp
        src/renderer.cpp
        src/texture.cpp
        )
add_executable(ParticleSystem ${SOURCE_FILES})

if (MSVC)
  set_target_properties(ParticleSystem PROPERTIES VS_DEBUGGER_WORKING_DIRECTORY "${CMAKE_SOURCE_DIR}")
  set_property(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR} PROPERTY VS_STARTUP_PROJECT ParticleSystem)
endif(MSVC)

target_link_libraries(ParticleSystem glfw ${GLFW_LIBRARIES})
target_link_libraries(ParticleSystem ${OPENGL_LIBRARIES})
target_link_libraries(ParticleSystem ${OPENCL_LIBRARIES})

