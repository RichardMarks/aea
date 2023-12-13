//
// Created by Richard Marks on 12/12/23.
//

#include "effect.h"
#include "file_utils.h"
#include "shader.h"

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

struct AEA_Effect *AEA_CreateDefaultEffect()
{
  struct AEA_Effect *effect = (struct AEA_Effect *)malloc(sizeof(struct AEA_Effect));
  
  AEA_InitDefaultEffect(effect);
  
  return effect;
}

struct AEA_Effect *AEA_CreateEffect(AEA_cstr vert_source, AEA_cstr frag_source)
{
  struct AEA_Effect *effect = (struct AEA_Effect *)malloc(sizeof(struct AEA_Effect));
  
  AEA_InitEffect(effect, vert_source, frag_source);
  
  return effect;
}

struct AEA_Effect *AEA_CreateEffectFromFiles(AEA_cstr vert_filename, AEA_cstr frag_filename)
{
  struct AEA_Effect *effect = (struct AEA_Effect *)malloc(sizeof(struct AEA_Effect));
  
  AEA_InitEffectFromFiles(effect, vert_filename, frag_filename);
  
  return effect;
}

AEA_void AEA_InitDefaultEffect(struct AEA_Effect *effect)
{
  AEA_cstr vert_source = "#version 330 core\nlayout (location = 0) in vec3 aPos;\nvoid main() { gl_Position = vec4(aPos, 1.0); }";
  
  AEA_cstr frag_source = "#version 330 core\nout vec4 FragColor;\nvoid main() { FragColor = vec4(1.0, 1.0, 1.0, 1.0); }";
  
  AEA_InitEffect(effect, vert_source, frag_source);
}

AEA_void AEA_InitEffect(struct AEA_Effect *effect, AEA_cstr vert_source, AEA_cstr frag_source)
{
  struct AEA_BuildShaderStatus status;
  effect->program_id = 0;
  
  AEA_u32 shader_program = AEA_BuildShaderProgram(vert_source, frag_source, &status);
  
  if (status.success)
  {
    effect->program_id = shader_program;
  }
}

AEA_void AEA_InitEffectFromFiles(struct AEA_Effect *effect, AEA_cstr vert_filename, AEA_cstr frag_filename)
{
  AEA_str vert_source = AEA_LoadEntireFile(vert_filename);
  AEA_str frag_source = AEA_LoadEntireFile(frag_filename);
  
  AEA_InitEffect(effect, vert_source, frag_source);
  
  free(vert_source);
  free(frag_source);
}

AEA_void AEA_DestroyEffect(struct AEA_Effect *effect)
{
  glDeleteProgram(effect->program_id);
}

AEA_void AEA_UseEffect(struct AEA_Effect *effect)
{
  glUseProgram(effect->program_id);
}

AEA_void AEA_SetEffectMVP(struct AEA_Effect *effect, AEA_cstr name, mat4 mat_mvp)
{
  AEA_SetEffectUniformMat4(effect, name, mat_mvp);
}

AEA_void AEA_SetEffectUniformInt(struct AEA_Effect *effect, AEA_cstr name, AEA_s32 value)
{
  
  GLint location = glGetUniformLocation(effect->program_id, name);
  glUniform1i(location, value);
}

AEA_void AEA_SetEffectUniformFloat(struct AEA_Effect *effect, AEA_cstr name, AEA_f32 value)
{
  GLint location = glGetUniformLocation(effect->program_id, name);
  glUniform1f(location, value);
}

AEA_void AEA_SetEffectUniformBool(struct AEA_Effect *effect, AEA_cstr name, AEA_s32 value)
{
  GLint location = glGetUniformLocation(effect->program_id, name);
  glUniform1i(location, value);
}

AEA_void AEA_SetEffectUniformVec3(struct AEA_Effect *effect, AEA_cstr name, vec3 value)
{
  GLint location = glGetUniformLocation(effect->program_id, name);
  glUniform4fv(location, 1, value);
}

AEA_void AEA_SetEffectUniformVec4(struct AEA_Effect *effect, AEA_cstr name, vec4 value)
{
  GLint location = glGetUniformLocation(effect->program_id, name);
  glUniform4fv(location, 1, value);
}

AEA_void
AEA_SetEffectUniformVec4Components(struct AEA_Effect *effect, AEA_cstr name, AEA_f32 x, AEA_f32 y, AEA_f32 z,
                                   AEA_f32 w)
{
  GLint location = glGetUniformLocation(effect->program_id, name);
  glUniform4f(location, x, y, z, w);
}

AEA_void AEA_SetEffectUniformMat4(struct AEA_Effect *effect, AEA_cstr name, mat4 value)
{
  GLint location = glGetUniformLocation(effect->program_id, name);
  glUniformMatrix4fv(location, 1, GL_FALSE, (const GLfloat *) value);
}
