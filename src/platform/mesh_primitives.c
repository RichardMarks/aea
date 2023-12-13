//
// Created by Richard Marks on 12/13/23.
//

#include <string.h>

#include "mesh_primitives.h"

AEA_void AEA_InitMesh(struct AEA_Mesh *mesh,
  AEA_size index_count,
  AEA_u32 *indices,
  AEA_size vertex_count,
  const AEA_f32 *vertex_positions,
  const AEA_f32 *vertex_normals,
  const AEA_f32 *vertex_colors,
  const AEA_f32 *vertex_uvs);

struct AEA_Mesh *AEA_CreatePlaneMesh(AEA_f32 width, AEA_f32 depth)
{
  struct AEA_Mesh *mesh = (struct AEA_Mesh *) malloc(sizeof(struct AEA_Mesh));
  memset(mesh, 0, sizeof(struct AEA_Mesh));
  
  AEA_InitPlaneMesh(mesh, width, depth);
  
  return mesh;
}

AEA_void AEA_InitPlaneMesh(struct AEA_Mesh *mesh, AEA_f32 width, AEA_f32 depth)
{
  if (!mesh)
  {
    fprintf(stderr, "unable to init plane mesh - no mesh\n");
    return;
  }
  
  // make sure the size is positive
  if (width < 0.0f) {
    width = -width;
  }
  
  if (depth < 0.0f)
  {
    depth = -depth;
  }
  
  // later we might want to have a subdivided plane
  // for now, just use a simple 4 vertex 2 triangle quad
  // colored full white, normals Y up, and uv-mapped as quad
  
  AEA_f32 vertex_positions[] = {
    -0.5f, 0.0f, -0.5f,
    0.5f, 0.0f, -0.5f,
    0.5f, 0.0f, 0.5f,
    -0.5f, 0.0f, 0.5f,
  };
  
  // scale the positions to fit size
  for (AEA_size i = 0; i < 4; i++)
  {
    vertex_positions[i * 3] *= width;
    vertex_positions[i * 3 + 2] *= depth;
  }
  
  AEA_f32 vertex_normals[] = {
    0.0f, 1.0f, 0.0f,
    0.0f, 1.0f, 0.0f,
    0.0f, 1.0f, 0.0f,
    0.0f, 1.0f, 0.0f,
  };
  
  AEA_f32 vertex_colors[] = {
    1.0f, 1.0f, 1.0f, 1.0f,
    1.0f, 1.0f, 1.0f, 1.0f,
    1.0f, 1.0f, 1.0f, 1.0f,
    1.0f, 1.0f, 1.0f, 1.0f,
  };
  
  AEA_f32 vertex_uvs[] = {
    0.0f, 1.0f,
    1.0f, 1.0f,
    1.0f, 0.0f,
    0.0f, 0.0f,
  };
  
  AEA_u32 indices[] = {0,1,2,0,2,3};
  
  AEA_InitMesh(mesh, 6, indices, 4, vertex_positions, vertex_normals, vertex_colors, vertex_uvs);
}


AEA_void AEA_InitMesh(struct AEA_Mesh *mesh,
                      AEA_size index_count,
                      AEA_u32 *indices,
                      AEA_size vertex_count,
                      const AEA_f32 *vertex_positions,
                      const AEA_f32 *vertex_normals,
                      const AEA_f32 *vertex_colors,
                      const AEA_f32 *vertex_uvs
)
{
  // if no mesh, cannot continue
  if (!mesh)
  {
    fprintf(stderr, "unable to init mesh - no mesh\n");
    return;
  }
  
  // if no indices, cannot continue
  if (!index_count)
  {
    fprintf(stderr, "unable to init mesh - no indices\n");
    return;
  }
  
  // if no vertex count, cannot continue
  if (!vertex_count)
  {
    fprintf(stderr, "unable to init mesh - no vertices\n");
    return;
  }
  
  // delete any existing data in the mesh
  if (mesh->vertices)
  {
    fprintf(stderr, "init mesh - freeing existing vertices\n");
    free(mesh->vertices);
    mesh->vertices = NULL;
  }
  
  if (mesh->indices)
  {
    fprintf(stderr, "init mesh - freeing existing indices\n");
    free(mesh->indices);
    mesh->indices = NULL;
  }
  
  // allocate space for the mesh
  mesh->vertex_count = vertex_count;
  mesh->index_count = index_count;
  
  fprintf(stderr, "init mesh - allocating space for %lu vertices and %lu indices\n", vertex_count, index_count);
  mesh->vertices = (struct AEA_Vertex *) malloc(sizeof(struct AEA_Vertex) * mesh->vertex_count);
  mesh->indices = (AEA_u32 *) malloc(sizeof(AEA_u32) * mesh->index_count);
  
  // copy the mesh data
  fprintf(stderr, "init mesh - copying indices\n");
  memcpy(mesh->indices, indices, sizeof(AEA_u32) * mesh->index_count);
  
  fprintf(stderr, "init mesh - copying vertices\n");
  for (AEA_size i = 0; i < mesh->vertex_count; i++)
  {
    struct AEA_Vertex *vtx = &mesh->vertices[i];

    AEA_size i2 = i * 2;
    AEA_size i3 = i * 3;
    AEA_size i4 = i * 4;
    
    // copy vertex position x, y, z
    vtx->position[0] = vertex_positions[i3];
    vtx->position[1] = vertex_positions[i3 + 1];
    vtx->position[2] = vertex_positions[i3 + 2];
    
    // copy vertex normal x, y, z
    vtx->normal[0] = vertex_normals[i3];
    vtx->normal[1] = vertex_normals[i3 + 1];
    vtx->normal[2] = vertex_normals[i3 + 2];
    
    // copy vertex color r, g, b, a
    vtx->color[0] = vertex_colors[i4];
    vtx->color[1] = vertex_colors[i4 + 1];
    vtx->color[2] = vertex_colors[i4 + 2];
    vtx->color[3] = vertex_colors[i4 + 3];
    
    // copy vertex texture coordinate u, v
    vtx->uv[0] = vertex_uvs[i2];
    vtx->uv[1] = vertex_uvs[i2 + 1];
  }
}