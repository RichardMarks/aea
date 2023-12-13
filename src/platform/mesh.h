//
// Created by Richard Marks on 12/12/23.
//

#ifndef AEA_MESH_H
#define AEA_MESH_H

#include <cglm/cglm.h>
#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "internal/platform_types.h"

struct AEA_Vertex {
  CGLM_ALIGN(16) vec3 position;
  CGLM_ALIGN(16) vec3 normal;
  CGLM_ALIGN(16) vec4 color;
  CGLM_ALIGN(8) vec2 uv;
};

struct AEA_Mesh {
  struct AEA_Vertex *vertices;
  AEA_u32 *indices;
  AEA_size vertex_count;
  AEA_size index_count;
};

struct AEA_MeshRenderer {
  AEA_u32 vao;
  AEA_u32 vbo;
  AEA_u32 ebo;
};

// extern struct AEA_MeshRenderer *AEA_CreateMeshRenderer();


#endif //AEA_MESH_H
