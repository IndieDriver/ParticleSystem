#pragma once
#include <algorithm>
#include <map>
#include <unordered_map>
#include <vector>
#include "env.hpp"
#include "part.hpp"
#include "shader.hpp"
#include "texture.hpp"

class Shader;

enum class PolygonMode { Point, Line, Fill };

struct Uniforms {
  glm::mat4 view;
  glm::mat4 proj;
  glm::mat4 view_proj;
};

class TextRenderer {
  struct Character {
    GLuint textureID;
    glm::ivec2 size;
    glm::ivec2 bearing;
    GLuint advanceOffset;
  };

 public:
  TextRenderer(void);
  ~TextRenderer();
  void renderText(float pos_x, float pos_y, float scale, std::string text,
                  glm::vec3 color, glm::mat4 ortho);

 private:
  std::map<GLchar, Character> _characters;
  GLuint _vao;
  GLuint _vbo;
  GLuint _shader_id;
};

class UiRenderer {
 public:
  UiRenderer(void);
  UiRenderer(UiRenderer const& src);
  ~UiRenderer(void);
  UiRenderer& operator=(UiRenderer const& rhs);
  void renderUI(std::string texture_name, float pos_x, float pos_y, float scale,
                glm::mat4 ortho, bool centered);

 private:
  GLuint _vao;
  GLuint _vbo;
  GLuint _shader_id;
  std::unordered_map<std::string, Texture*> _texture_cache;
};

class Renderer {
 public:
  Renderer(void);
  Renderer(int width, int height);
  Renderer(Renderer const& src);
  virtual ~Renderer(void);
  Renderer& operator=(Renderer const& rhs);
  void renderText(float pos_x, float pos_y, float scale, std::string text,
                  glm::vec3 color);
  void renderUI(std::string filename, float pos_x, float pos_y, float scale,
                bool centered);
  void update(const Env& env);
  void draw();
  void flush();
  void reset();
  int getScreenWidth();
  int getScreenHeight();
  void loadCubeMap(std::string vertex_sha, std::string fragment_sha,
                   const std::vector<std::string>& textures);
  void clearScreen();
  void switchPolygonMode(enum PolygonMode mode);
  void setDepthtest(bool val);
  void setBlend(bool val);
  Uniforms uniforms;

 private:
  int _width;
  int _height;
  enum PolygonMode _polygonMode;
  TextRenderer _textRenderer;
  UiRenderer _uiRenderer;
  bool _depthTest = false;
  bool _blend = true;
  void switchShader(GLuint shader_id, int& current_shader_id);
};

static inline void setUniform(const GLint& location, const float& data) {
  glUniform1f(location, data);
}
static inline void setUniform(const GLint& location, const int& data) {
  glUniform1i(location, data);
}
static inline void setUniform(const GLint& location, const glm::vec2& data) {
  glUniform2fv(location, 1, static_cast<const GLfloat*>(glm::value_ptr(data)));
}
static inline void setUniform(const GLint& location, const glm::vec3& data) {
  glUniform3fv(location, 1, static_cast<const GLfloat*>(glm::value_ptr(data)));
}
static inline void setUniform(const GLint& location, const glm::vec4& data) {
  glUniform4fv(location, 1, static_cast<const GLfloat*>(glm::value_ptr(data)));
}
static inline void setUniform(const GLint& location, const glm::mat4& data) {
  glUniformMatrix4fv(location, 1, GL_FALSE,
                     static_cast<const GLfloat*>(glm::value_ptr(data)));
}
