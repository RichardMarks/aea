//
// Created by Richard Marks on 12/13/23.
//

#ifndef AEA_SHADER_H
#define AEA_SHADER_H

#include "internal/platform_internal.h"

#define AEA_BUILD_SHADER_STATUS_INFO_LOG_SIZE 0x400

struct AEA_BuildShaderStatus {
  int success;
  char info_log[AEA_BUILD_SHADER_STATUS_INFO_LOG_SIZE];
};

extern AEA_u32 AEA_CompileVertexShader(AEA_cstr vert_source, struct AEA_BuildShaderStatus *status);
extern AEA_u32 AEA_CompileFragmentShader(AEA_cstr frag_source, struct AEA_BuildShaderStatus *status);
extern AEA_u32 AEA_LinkShaderProgram(AEA_u32 vertex_shader, AEA_u32 fragment_shader, struct AEA_BuildShaderStatus *status);
extern AEA_u32 AEA_BuildShaderProgram(AEA_cstr vert_source, AEA_cstr frag_source, struct AEA_BuildShaderStatus *status);

#endif //AEA_SHADER_H
