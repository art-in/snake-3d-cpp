#include "opengl.hpp"

auto initShader(GLenum shader_type, const std::string& shader_src) -> GLuint {
  auto shader = glCreateShader(shader_type);
  const char* shader_src_c_str = shader_src.c_str();
  glShaderSource(shader, 1, &shader_src_c_str, nullptr);
  glCompileShader(shader);

  GLint compile_status{};
  int info_log_length{};

  glGetShaderiv(shader, GL_COMPILE_STATUS, &compile_status);
  glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &info_log_length);

  if (compile_status == GL_FALSE) {
    std::vector<char> log_record(info_log_length + 1);
    glGetShaderInfoLog(shader, info_log_length, nullptr, &log_record[0]);

    std::string error_message =
        "Failed to compile shader:\n" + std::string{&log_record[0]};

    glDeleteShader(shader);

    emscripten_throw_string(error_message.c_str());
  }

  return shader;
}

auto initProgram(const std::vector<GLuint>& shaders) -> GLuint {
  auto program = glCreateProgram();

  for (const auto shader : shaders) {
    glAttachShader(program, shader);
  }

  glLinkProgram(program);

  GLint link_status{};
  int info_log_length{};

  glGetProgramiv(program, GL_LINK_STATUS, &link_status);
  glGetProgramiv(program, GL_INFO_LOG_LENGTH, &info_log_length);

  if (link_status == GL_FALSE) {
    std::vector<char> log_record(info_log_length + 1);
    glGetProgramInfoLog(program, info_log_length, nullptr, &log_record[0]);

    std::string error_message =
        "Failed to link program:\n" + std::string{&log_record[0]};

    glDeleteProgram(program);

    emscripten_throw_string(error_message.c_str());
  }

  return program;
}

auto getAttributeLocation(GLuint program, const char* attribute_name) -> GLint {
  const auto location = glGetAttribLocation(program, attribute_name);
  if (location == -1) {
    const auto error_message =
        std::string{"Failed to get attribute location: "} + attribute_name;
    emscripten_throw_string(error_message.c_str());
  }
  return location;
}

auto getUniformLocation(GLuint program, const char* uniform_name) -> GLint {
  const auto location = glGetUniformLocation(program, uniform_name);
  if (location == -1) {
    const auto error_message =
        std::string{"Failed to get uniform location: "} + uniform_name;
    emscripten_throw_string(error_message.c_str());
  }
  return location;
}
