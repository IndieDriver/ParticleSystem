//
// Created by amath on 9/28/2016.
//

#include <fstream>
#include "GLenv.h"

const char* GL_type_to_string (GLenum type) {
    switch (type) {
        case GL_BOOL: return "bool";
        case GL_INT: return "int";
        case GL_FLOAT: return "float";
        case GL_FLOAT_VEC2: return "vec2";
        case GL_FLOAT_VEC3: return "vec3";
        case GL_FLOAT_VEC4: return "vec4";
        case GL_FLOAT_MAT2: return "mat2";
        case GL_FLOAT_MAT3: return "mat3";
        case GL_FLOAT_MAT4: return "mat4";
        case GL_SAMPLER_2D: return "sampler2D";
        case GL_SAMPLER_3D: return "sampler3D";
        case GL_SAMPLER_CUBE: return "samplerCube";
        case GL_SAMPLER_2D_SHADOW: return "sampler2DShadow";
        default: break;
    }
    return "other";
}

void _print_programme_info_log (GLuint programme) {
    int max_length = 2048;
    int actual_length = 0;
    char log[2048];
    glGetProgramInfoLog (programme, max_length, &actual_length, log);
    printf ("program info log for GL index %u:\n%s", programme, log);
}

void print_all (GLuint programme) {
    printf ("--------------------\nshader programme %i info:\n", programme);
    int params = -1;
    glGetProgramiv (programme, GL_LINK_STATUS, &params);
    printf ("GL_LINK_STATUS = %i\n", params);

    glGetProgramiv (programme, GL_ATTACHED_SHADERS, &params);
    printf ("GL_ATTACHED_SHADERS = %i\n", params);

    glGetProgramiv (programme, GL_ACTIVE_ATTRIBUTES, &params);
    printf ("GL_ACTIVE_ATTRIBUTES = %i\n", params);
    for (int i = 0; i < params; i++) {
        char name[64];
        int max_length = 64;
        int actual_length = 0;
        int size = 0;
        GLenum type;
        glGetActiveAttrib (
                programme,
                i,
                max_length,
                &actual_length,
                &size,
                &type,
                name
        );
        if (size > 1) {
            for (int j = 0; j < size; j++) {
                char long_name[64];
                sprintf (long_name, "%s[%i]", name, j);
                int location = glGetAttribLocation (programme, long_name);
                printf ("  %i) type:%s name:%s location:%i\n",
                        i, GL_type_to_string (type), long_name, location);
            }
        } else {
            int location = glGetAttribLocation (programme, name);
            printf ("  %i) type:%s name:%s location:%i\n",
                    i, GL_type_to_string (type), name, location);
        }
    }

    glGetProgramiv (programme, GL_ACTIVE_UNIFORMS, &params);
    printf ("GL_ACTIVE_UNIFORMS = %i\n", params);
    for (int i = 0; i < params; i++) {
        char name[64];
        int max_length = 64;
        int actual_length = 0;
        int size = 0;
        GLenum type;
        glGetActiveUniform (
                programme,
                i,
                max_length,
                &actual_length,
                &size,
                &type,
                name
        );
        if (size > 1) {
            for (int j = 0; j < size; j++) {
                char long_name[64];
                sprintf (long_name, "%s[%i]", name, j);
                int location = glGetUniformLocation (programme, long_name);
                printf ("  %i) type:%s name:%s location:%i\n",
                        i, GL_type_to_string (type), long_name, location);
            }
        } else {
            int location = glGetUniformLocation (programme, name);
            printf ("  %i) type:%s name:%s location:%i\n",
                    i, GL_type_to_string (type), name, location);
        }
    }
    _print_programme_info_log(programme);
}

void	print_link_error(GLuint program, std::string vfn, std::string ffn)
{
    char	log[2048];
    int		max_length;
    int		index;

    max_length = 2048;
    index = 0;
    glGetProgramInfoLog(program, max_length, &index, log);
    glGetShaderInfoLog(program, max_length, &index, log);
    std::cout << "Cannot link shaders: " << vfn << " and " << ffn << "\n" << log;
    exit(0);
}

void	print_shader_error(GLuint shader, std::string file_name)
{
    char	log[2048];
    int		max_length;
    int		index;

    max_length = 2048;
    index = 0;
    glGetShaderInfoLog(shader, max_length, &index, log);
    std::cout << "Cannot compile shader: " << file_name << "\n" << log;
    exit(0);
}

bool is_valid (GLuint programme) {
    glValidateProgram (programme);
    int params = -1;
    glGetProgramiv (programme, GL_VALIDATE_STATUS, &params);
    printf ("program %i GL_VALIDATE_STATUS = %i\n", programme, params);
    if (GL_TRUE != params) {
        _print_programme_info_log (programme);
        return false;
    }
    return true;
}

GLenv::GLenv(std::string fragment_fn, std::string vertex_fn) {
    std::string line;
    std::string fragment = "";
    int		error;
    GLuint	fs;
    GLuint	vs;
    std::fstream file_fragment(fragment_fn);
    if (file_fragment)
    {
        while (getline(file_fragment,line))
            fragment += line + "\n";
        file_fragment.close();
    } else
        std::cout << "invalid file_name\n" ;

    std::string vertex = "";
    std::ifstream file_vertex(vertex_fn);
    if (file_vertex)
    {
        while (getline(file_vertex,line))
            vertex += line + "\n";
        file_vertex.close();
    } else
        std::cout << "invalid file_name\n" ;

    error = -1;
    vs = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vs, 1, (const char**)&vertex, NULL);
    glCompileShader(vs);
    glGetShaderiv(vs, GL_COMPILE_STATUS, &error);
    if (GL_TRUE != error)
        print_shader_error(vs, vertex_fn);
    fs = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fs, 1,(const char**)&fragment, NULL);
    glCompileShader(fs);
    glGetShaderiv(fs, GL_COMPILE_STATUS, &error);
    if (GL_TRUE != error)
        print_shader_error(fs, fragment_fn);
    this->program_id = glCreateProgram();
    glAttachShader(this->program_id, fs);
    glAttachShader(this->program_id, vs);
    glLinkProgram(this->program_id);
    glGetProgramiv(this->program_id, GL_LINK_STATUS, &error);
    if (GL_TRUE != error)
        print_link_error(this->program_id, vertex_fn, fragment_fn);
    glDeleteShader(vs);
    glDeleteShader(fs);
    //print_all(this->program_id);
    //is_valid(this->program_id);
    glUseProgram(this->program_id);
}
