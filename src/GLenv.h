#ifndef PARTICLESYSTEM_GLENV_H
#define PARTICLESYSTEM_GLENV_H

#include "part.h"

class GLenv {
    public:
        GLenv(std::string fragment_fn, std::string vertex_fn);
        GLuint program_id = 0;
};


#endif //PARTICLESYSTEM_GLENV_H
