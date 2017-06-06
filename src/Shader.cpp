#include "Shader.h"

Shader::Shader(std::string fragmentFilename, std::string vertexFilename) {
    GLuint  vertexID;
    GLuint  fragID;
    GLint   err = -1;

    vertexID = compileShader(vertexFilename, GL_VERTEX_SHADER);
    fragID = compileShader(fragmentFilename, GL_FRAGMENT_SHADER);
    id = linkShaders(vertexID, fragID);
    glUseProgram(id);
    glGetProgramiv(id, GL_LINK_STATUS, &err);
    if (GL_TRUE != err)
	    printLinkError(id, vertexFilename, fragmentFilename);
}

GLuint Shader::linkShaders(GLuint vertexID, GLuint fragID) {
	GLuint id = glCreateProgram();

    glAttachShader(id, vertexID);
    glAttachShader(id, fragID);
    glLinkProgram(id);
    glDeleteShader(vertexID);
    glDeleteShader(fragID);
    return (id);
}

GLuint Shader::compileShader(std::string fileName, GLuint shaderType){
	GLuint id;
	std::string line;
	std::string fileContent = "";
	int err = -1;

	std::fstream shaderFile(fileName);
	if (shaderFile)
	{
		while (getline(shaderFile, line))
			fileContent += line + "\n";
		shaderFile.close();
	} else
		std::cout << "Invalid shader file name: " << fileName << "\n";
	id = glCreateShader(shaderType);
	const char *fileContentChar = fileContent.c_str();
	glShaderSource(id, 1, &fileContentChar, NULL);
	glCompileShader(id);
	glGetShaderiv(id, GL_COMPILE_STATUS, &err);
	if (GL_TRUE != err)
		printShaderError(id, fileName);
	return (id);
}

void	printLinkError(GLuint program, std::string vfn, std::string ffn)
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

void	printShaderError(GLuint shader, std::string file_name)
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
