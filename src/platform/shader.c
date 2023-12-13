//
// Created by Richard Marks on 12/13/23.
//

#include "shader.h"

#include <stdio.h>

AEA_u32 AEA_CompileVertexShader(AEA_cstr vert_source, struct AEA_BuildShaderStatus *status)
{
  fprintf(stderr, "compiling vertex shader\n%s\n", vert_source);
  AEA_u32 vertex_shader = glCreateShader(GL_VERTEX_SHADER);
  
  glShaderSource(vertex_shader, 1, &vert_source, NULL);
  glCompileShader(vertex_shader);
  
  glGetShaderiv(vertex_shader, GL_COMPILE_STATUS, &status->success);
  
  if (!status->success) {
    glGetShaderInfoLog(vertex_shader, AEA_BUILD_SHADER_STATUS_INFO_LOG_SIZE, NULL, status->info_log);
  }
  
  return vertex_shader;
}

AEA_u32 AEA_CompileFragmentShader(AEA_cstr frag_source, struct AEA_BuildShaderStatus *status)
{
  fprintf(stderr, "compiling fragment shader\n%s\n", frag_source);
  AEA_u32 fragment_shader = glCreateShader(GL_FRAGMENT_SHADER);
  
  glShaderSource(fragment_shader, 1, &frag_source, NULL);
  glCompileShader(fragment_shader);
  
  glGetShaderiv(fragment_shader, GL_COMPILE_STATUS, &status->success);
  
  if (!status->success) {
    glGetShaderInfoLog(fragment_shader, AEA_BUILD_SHADER_STATUS_INFO_LOG_SIZE, NULL, status->info_log);
  }
  
  return fragment_shader;
}

AEA_u32 AEA_LinkShaderProgram(AEA_u32 vertex_shader, AEA_u32 fragment_shader, struct AEA_BuildShaderStatus *status)
{
  AEA_u32 program_id = glCreateProgram();
  
  glAttachShader(program_id, vertex_shader);
  glAttachShader(program_id, fragment_shader);
  glLinkProgram(program_id);
  
  glGetProgramiv(program_id, GL_LINK_STATUS, &status->success);
  if (!status->success)
  {
    glGetProgramInfoLog(program_id, AEA_BUILD_SHADER_STATUS_INFO_LOG_SIZE, NULL, status->info_log);
  }
  
  return program_id;
}

AEA_u32 AEA_BuildShaderProgram(AEA_cstr vert_source, AEA_cstr frag_source, struct AEA_BuildShaderStatus *status)
{
  AEA_u32 vertex_shader = AEA_CompileVertexShader(vert_source, status);
  
  if (!status->success) {
    fprintf(stderr, "failed to compile vertex shader\n%s", status->info_log);
    return 0;
  }
  
  AEA_u32 fragment_shader = AEA_CompileFragmentShader(frag_source, status);
  
  if (!status->success) {
    fprintf(stderr, "failed to compile fragment shader\n%s", status->info_log);
    return 0;
  }
  
  AEA_u32 program_id = AEA_LinkShaderProgram(vertex_shader, fragment_shader, status);
  
  if (!status->success) {
    fprintf(stderr, "failed to link shader\n%s", status->info_log);
    return 0;
  }
  
  glDeleteShader(vertex_shader);
  glDeleteShader(fragment_shader);
  
  return program_id;
}
