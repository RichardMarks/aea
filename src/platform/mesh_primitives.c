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
  struct AEA_Mesh *mesh = AEA_CreateMesh();
  
  AEA_InitPlaneMesh(mesh, width, depth);
  
  return mesh;
}

struct AEA_Mesh *AEA_CreateHemisphereMesh(AEA_f32 radius, AEA_s32 num_segments, AEA_s32 num_rings)
{
  struct AEA_Mesh *mesh = AEA_CreateMesh();
  
  AEA_InitHemisphereMesh(mesh, radius, num_segments, num_rings);
  
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

AEA_void AEA_InitHemisphereMesh(struct AEA_Mesh *mesh, AEA_f32 radius, AEA_s32 num_segments, AEA_s32 num_rings)
{
  if (!mesh)
  {
    fprintf(stderr, "unable to init hemisphere mesh - no mesh\n");
    return;
  }
  
  // make sure that radius is positive
  if (radius < 0.0f)
  {
    radius = -radius;
  }
  
  // make sure that the number of segments and rings make sense
  if (num_segments < 3)
  {
    num_segments = 3;
  }
  
  if (num_rings < 3)
  {
    num_rings = 3;
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
  
  // segments are the number of sides around the dome
  // rings are the number of vertical divisions
  
  mesh->vertex_count = (num_segments + 1) * (num_rings + 1);
  mesh->index_count = (num_segments * (num_rings + 1) * 2);
  
  fprintf(stderr, "init hemisphere mesh - allocating space for %lu vertices and %lu indices\n", mesh->vertex_count, mesh->index_count);
  mesh->vertices = (struct AEA_Vertex *) malloc(sizeof(struct AEA_Vertex) * mesh->vertex_count);
  
  if (!mesh->vertices)
  {
    fprintf(stderr, "unable to allocate memory for hemisphere mesh vertices");
    return;
  } else {
    fprintf(stderr, "vertices allocated\n");
  }
  
  mesh->indices = (AEA_u32 *) malloc(sizeof(AEA_u32) * mesh->index_count);
  
  if (!mesh->indices)
  {
    free(mesh->vertices);
    fprintf(stderr, "unable to allocate memory for hemisphere mesh indices\n");
    return;
  }else {
    fprintf(stderr, "indices allocated\n");
  }
  
  AEA_f32 dampening_factor = 1.0f; // make a parameter?
  
  AEA_f32 theta_angle = (AEA_f32)(2.0 * M_PI / num_segments);
  for (AEA_s32 j = 0; j <= num_rings; j++)
  {
    AEA_f32 jf32 = (AEA_f32)j;
    AEA_f32 phi_angle = (AEA_f32)(j * ((M_PI * 0.5) / num_rings));
    for (AEA_s32 i = 0; i <= num_segments; i++) {
      AEA_f32 if32 = (AEA_f32)i;
      AEA_f32 x = cosf(if32 * theta_angle) * cosf(phi_angle);
      AEA_f32 y = dampening_factor * sinf(phi_angle);
      AEA_f32 z = sinf(if32 * theta_angle) * cosf(phi_angle);
      
      AEA_size vertex_index = j * (num_segments + 1) + i;
      if (vertex_index >= mesh->vertex_count)
      {
        fprintf(stderr, "VERTEX INDEX OUT OF BOUNDS j=%d i=%d vertex index %lu is > %lu\n", j, i, vertex_index, mesh->vertex_count - 1);
      }
      struct AEA_Vertex *vtx = &mesh->vertices[vertex_index];
      vtx->position[0] = x * radius;
      vtx->position[1] = y * radius;
      vtx->position[2] = z * radius;
      
      // need to figure out the correct normals...
      vtx->normal[0] = x;
      vtx->normal[1] = y;
      vtx->normal[2] = z;
      glm_vec3_norm(vtx->normal);
      
      // default white
      vtx->color[0] = 1.0f * (jf32 / (AEA_f32)num_rings);
      vtx->color[1] = 1.0f * (if32 / (AEA_f32)num_segments);
      vtx->color[2] = 1.0f * (jf32 / (AEA_f32)num_rings);
      vtx->color[3] = 1.0f;
      
      // texture coordinates
      vtx->uv[0] = if32 / (AEA_f32)num_segments;
      vtx->uv[1] = jf32 / (AEA_f32)num_rings;
    }
  }
  
  AEA_size n = 0;
  for (AEA_s32 j = 1; j <= num_segments; j++)
  {
    for (AEA_s32 i = 0; i <= num_rings; i++)
    {
      mesh->indices[n++] = j * (num_segments + 1) + i;
      mesh->indices[n++] = (j - 1) * (num_segments + 1) + i;
    }
  }
  fprintf(stderr, "init hemisphere finished\n");
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