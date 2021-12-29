#pragma once

#include <GLES2/gl2.h>
#include <emscripten.h>

#include <string>
#include <vector>

auto initShader(GLenum shader_type, const std::string& shader_src) -> GLuint;
auto initProgram(const std::vector<GLuint>& shaders) -> GLuint;
auto getAttributeLocation(GLuint program, const char* attribute_name) -> GLint;
auto getUniformLocation(GLuint program, const char* uniform_name) -> GLint;
