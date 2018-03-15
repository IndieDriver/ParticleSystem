#pragma once
#include <OpenGL/OpenGL.h>
#define __CL_ENABLE_EXCEPTIONS
#define CL_HPP_ENABLE_EXCEPTIONS
#define WIDTH 1280
#define HEIGHT 720
#define PARTICLE_NUM 1000000
//#define M_PI 3.14159265358979323846
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/color_space.hpp>
#include <glm/gtx/euler_angles.hpp>
#include <glm/gtx/transform.hpp>
#include "clenv.hpp"
//#include "env.hpp"

#include <math.h>
#include <cassert>
#include <fstream>
#include <iostream>
#include <string>
