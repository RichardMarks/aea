//
// Created by Richard Marks on 12/12/23.
//

#ifndef AEA_MESH_H
#define AEA_MESH_H

#include <cglm/cglm.h>
#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "internal/platform_types.h"
#include "effect.h"

struct AEA_Vertex {
  CGLM_ALIGN(16) vec3 position;
  CGLM_ALIGN(16) vec3 normal;
  CGLM_ALIGN(16) vec4 color;
  CGLM_ALIGN(8) vec2 uv;
};

// any mesh with this flag is assumed to have been allocated by AEA_CreateMesh
// and is required for AEA_DestroyMesh to free the allocation
#define AEA_MESH_FLAG_DYNAMIC 0x6c40

struct AEA_Mesh {
  AEA_s32 flags;
  struct AEA_Vertex *vertices;
  AEA_u32 *indices;
  AEA_size vertex_count;
  AEA_size index_count;
};

#define AEA_MESH_RENDERER_STRIP 0x5

struct AEA_MeshRenderer {
  AEA_s32 flags;
  AEA_u32 vao;
  AEA_u32 vbo;
  AEA_u32 ebo;
  AEA_u32 texture;
  struct AEA_Mesh *mesh;
};

extern struct AEA_Mesh *AEA_CreateMesh();

extern AEA_void AEA_DestroyMesh(struct AEA_Mesh *mesh);

extern struct AEA_MeshRenderer *AEA_CreateMeshRenderer();
extern AEA_void AEA_DestroyMeshRenderer(struct AEA_MeshRenderer *mesh_renderer);
extern AEA_void AEA_BindMeshMeshRendererMesh(struct AEA_MeshRenderer *mesh_renderer, struct AEA_Mesh *mesh);
extern AEA_void AEA_BindMeshRendererTexture(struct AEA_MeshRenderer *mesh_renderer, AEA_s32 width, AEA_s32 height, AEA_u8 *pixels);

extern AEA_void AEA_DrawMeshRenderer(struct AEA_MeshRenderer *mesh_renderer, struct AEA_Effect *effect, mat4 mat_mvp);

extern AEA_void AEA_InitMeshFromFile(struct AEA_Mesh *mesh, AEA_cstr filename);
extern AEA_void AEA_WriteMeshToFile(struct AEA_Mesh *mesh, AEA_cstr filename);

#endif //AEA_MESH_H
