//
// Created by Richard Marks on 12/12/23.
//

#include <stdlib.h>
#include <string.h>

#include "mesh.h"
#include "platform.h"

struct AEA_Mesh *AEA_CreateMesh()
{
  struct AEA_Mesh *mesh = (struct AEA_Mesh *)malloc(sizeof(struct AEA_Mesh));
  if (mesh)
  {
    memset(mesh, 0, sizeof(struct AEA_Mesh));
    mesh->flags |= AEA_MESH_FLAG_DYNAMIC;
  }
  return mesh;
}

AEA_void AEA_DestroyMesh(struct AEA_Mesh *mesh)
{
  if (mesh)
  {
    if (mesh->vertices)
    {
      free(mesh->vertices);
    }
    
    if (mesh->indices)
    {
      free(mesh->indices);
    }
    
    if (mesh->flags & AEA_MESH_FLAG_DYNAMIC)
    {
      free(mesh);
    }
  }
}

struct AEA_MeshRenderer *AEA_CreateMeshRenderer()
{
  struct AEA_MeshRenderer *mesh_renderer = (struct AEA_MeshRenderer *)malloc(sizeof(struct AEA_MeshRenderer));
  if (mesh_renderer)
  {
    memset(mesh_renderer, 0, sizeof(struct AEA_MeshRenderer));
    mesh_renderer->flags |= AEA_MESH_FLAG_DYNAMIC;
  }
  return mesh_renderer;
}

AEA_void AEA_DestroyMeshRenderer(struct AEA_MeshRenderer *mesh_renderer)
{
  if (mesh_renderer)
  {
    if (glIsVertexArray(mesh_renderer->vao))
    {
      glDeleteVertexArrays(1, &mesh_renderer->vao);
    }
    
    if (glIsBuffer(mesh_renderer->vbo))
    {
      glDeleteBuffers(1, &mesh_renderer->vbo);
    }
    
    if (glIsBuffer(mesh_renderer->ebo))
    {
      glDeleteBuffers(1, &mesh_renderer->ebo);
    }
    
    if (glIsTexture(mesh_renderer->texture))
    {
      glDeleteTextures(1, &mesh_renderer->texture);
    }
    
    if (mesh_renderer->flags & AEA_MESH_FLAG_DYNAMIC)
    {
      free(mesh_renderer);
    }
  }
}

AEA_void AEA_BindMeshMeshRendererMesh(struct AEA_MeshRenderer *mesh_renderer, struct AEA_Mesh *mesh)
{
  
  if (!mesh_renderer || !mesh)
  {
    return;
  }
  
  fprintf(stderr, "binding mesh with %lu vertices and %lu indices to mesh renderer\n", mesh->vertex_count, mesh->index_count);
  
  mesh_renderer->mesh = mesh;
  
  glGenVertexArrays(1, &mesh_renderer->vao);
  AEA_CheckGLError();
  glGenBuffers(1, &mesh_renderer->vbo);
  AEA_CheckGLError();
  glGenBuffers(1, &mesh_renderer->ebo);
  AEA_CheckGLError();
  glBindVertexArray(mesh_renderer->vao);
  AEA_CheckGLError();
  
  glBindBuffer(GL_ARRAY_BUFFER, mesh_renderer->vbo);
  AEA_CheckGLError();
  glBufferData(GL_ARRAY_BUFFER, sizeof(struct AEA_Vertex) * mesh->vertex_count, mesh->vertices, GL_STATIC_DRAW);
  AEA_CheckGLError();
  
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mesh_renderer->ebo);
  AEA_CheckGLError();
  glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(AEA_u32) * mesh->index_count, mesh->indices, GL_STATIC_DRAW);
  AEA_CheckGLError();
  
  glEnableVertexAttribArray(0);
  AEA_CheckGLError();
  glEnableVertexAttribArray(1);
  AEA_CheckGLError();
  glEnableVertexAttribArray(2);
  AEA_CheckGLError();
  glEnableVertexAttribArray(3);
  AEA_CheckGLError();
  
  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(struct AEA_Vertex),(AEA_any)offsetof(struct AEA_Vertex, position));
  AEA_CheckGLError();
  glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(struct AEA_Vertex),(AEA_any)offsetof(struct AEA_Vertex, normal));
  AEA_CheckGLError();
  glVertexAttribPointer(2, 4, GL_FLOAT, GL_FALSE, sizeof(struct AEA_Vertex),(AEA_any)offsetof(struct AEA_Vertex, color));
  AEA_CheckGLError();
  glVertexAttribPointer(3, 2, GL_FLOAT, GL_FALSE, sizeof(struct AEA_Vertex),(AEA_any)offsetof(struct AEA_Vertex, uv));
  AEA_CheckGLError();
}

AEA_void AEA_BindMeshRendererTexture(struct AEA_MeshRenderer *mesh_renderer, AEA_s32 width, AEA_s32 height, AEA_u8 *pixels)
{
  if (glIsTexture(mesh_renderer->texture) == GL_FALSE)
  {
    glGenTextures(1, &mesh_renderer->texture);
    AEA_CheckGLError();
  }
  glBindTexture(GL_TEXTURE_2D, mesh_renderer->texture);
  AEA_CheckGLError();
  
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
  AEA_CheckGLError();
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
  AEA_CheckGLError();
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
  AEA_CheckGLError();
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
  AEA_CheckGLError();
  glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, 2, 2, 0, GL_RGBA, GL_UNSIGNED_BYTE, pixels);
  AEA_CheckGLError();
  glGenerateMipmap(GL_TEXTURE_2D);
  AEA_CheckGLError();
}


AEA_void AEA_DrawMeshRenderer(struct AEA_MeshRenderer *mesh_renderer, struct AEA_Effect *effect, mat4 mat_mvp)
{
  if (!mesh_renderer)
  {
    fprintf(stderr, "unable to draw null mesh renderer\n");
    return;
  }
  
  if (!effect)
  {
    fprintf(stderr, "unable to draw mesh renderer without an effect\n");
    return;
  }
  
  if (!mesh_renderer->mesh)
  {
    fprintf(stderr, "unable to draw mesh renderer without a bound mesh\n");
    return;
  }
  
  glBindVertexArray(mesh_renderer->vao);
  AEA_CheckGLError();
  
  if (glIsTexture(mesh_renderer->texture) == GL_TRUE)
  {
    glBindTexture(GL_TEXTURE_2D, mesh_renderer->texture);
    AEA_CheckGLError();
  }
  
  AEA_UseEffect(effect);
  AEA_SetEffectMVP(effect, "mvp", mat_mvp);
  if (mesh_renderer->flags & AEA_MESH_RENDERER_STRIP)
  {
    glDrawElements(GL_TRIANGLE_STRIP, (GLsizei) mesh_renderer->mesh->index_count, GL_UNSIGNED_INT, 0);
  }
  else
  {
    glDrawElements(GL_TRIANGLES, (GLsizei) mesh_renderer->mesh->index_count, GL_UNSIGNED_INT, 0);
  }
  AEA_CheckGLError();
  
  glBindVertexArray(0);
  AEA_CheckGLError();
}

AEA_void AEA_InitMeshFromFile(struct AEA_Mesh *mesh, AEA_cstr filename)
{
  if (!mesh || !filename)
  {
    return;
  }
  
  // delete any existing data in the mesh
  if (mesh->vertices)
  {
    fprintf(stderr, "init mesh from file - freeing existing vertices\n");
    free(mesh->vertices);
    mesh->vertices = NULL;
  }
  
  if (mesh->indices)
  {
    fprintf(stderr, "init mesh from file - freeing existing indices\n");
    free(mesh->indices);
    mesh->indices = NULL;
  }
  
  FILE *fp = fopen(filename, "r");
  if (!fp)
  {
    fprintf(stderr, "unable to read from file %s\n", filename);
    return;
  }
  
  
  
  fclose(fp);
  
}

AEA_void AEA_WriteMeshToFile(struct AEA_Mesh *mesh, AEA_cstr filename)
{
  if (!mesh || !filename)
  {
    return;
  }
  
  FILE* fp = fopen(filename, "w");
  if (!fp)
  {
    return;
  }
  
  fprintf(fp, "# %s\n", filename);
  fprintf(fp, "# vertices: %lu\n# indices: %lu\n", mesh->vertex_count, mesh->index_count);
  for (AEA_size i = 0; i < mesh->vertex_count; i++)
  {
    struct AEA_Vertex *vtx = &mesh->vertices[i];
    fprintf(fp, "v %f %f %f\n", vtx->position[0], vtx->position[1], vtx->position[2]);
  }
  
  for (AEA_size i = 0; i < mesh->vertex_count; i++)
  {
    struct AEA_Vertex *vtx = &mesh->vertices[i];
    fprintf(fp, "vn %f %f %f\n", vtx->normal[0], vtx->normal[1], vtx->normal[2]);
  }
  
  for (AEA_size i = 0; i < mesh->vertex_count; i++)
  {
    struct AEA_Vertex *vtx = &mesh->vertices[i];
    fprintf(fp, "#vc %f %f %f %f\n", vtx->color[0], vtx->color[1], vtx->color[2], vtx->color[3]);
  }
  
  for (AEA_size i = 0; i < mesh->vertex_count; i++)
  {
    struct AEA_Vertex *vtx = &mesh->vertices[i];
    fprintf(fp, "vt %f %f\n", vtx->uv[0], vtx->uv[1]);
  }
  
  for (AEA_size i = 0; i < mesh->index_count; i += 3)
  {
    AEA_u32 v0 = mesh->indices[i] + 1;
    AEA_u32 v1 = mesh->indices[i + 1] + 1;
    AEA_u32 v2 = mesh->indices[i + 2] + 1;
    fprintf(fp, "f %u/%u/%u %u/%u/%u %u/%u/%u\n", v0, v0, v0, v1, v1, v1, v2, v2, v2);
  }
  
  fclose(fp);
}