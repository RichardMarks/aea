#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <cglm/cglm.h>

#include "platform/platform.h"
#include "platform/effect.h"
#include "platform/mesh.h"

#include "game_camera.h"
/*
 * Coding Convention:
 *
 * Use snake_case for variable names
 *
 * Use UPPERCASE_SNAKE_CASE for #defined constants
 *
 * Use AEA_ prefixed types eg, use AEA_s32 instead of int and AEA_f64 instead of double
 *
 * Use prefixed AEA_ PascalCase for struct names for AEA structs and PascalCase for game structs
 *
 * Use prefixed AEA_ PascalCase for function names for AEA functions and PascalCase for game functions
 *
 * Use curly braces always in single line if statements
 *
 * Use newlines after parentheses in functions, for and if statements
 *
 * Take care to align your cglm variables as follows
 * vec3: 8 byte
 * vec4: 16 byte
 * mat4: 16 byte
 * */


struct Game {
  struct GameCamera camera;
  mat4 mat_view;
  mat4 mat_projection;
};

int main()
{
  printf("AEA v0.1 (c) 2024 Richard Marks, Rambling Indie Games\n");
  
  struct AEA_CreatePlatformParams create_params;
  create_params.window_title = "AEA";
  create_params.window_width = 1920;
  create_params.window_height = 1080;
  create_params.fullscreen = 0;
  
  struct AEA_Platform *platform = AEA_CreatePlatform(&create_params);
  
  if (!platform)
  {
    fprintf(stderr, "platform initialization failed... Are you trying to run this on a potato?\n");
    exit(-1);
  }
  
  // do initialization here
  struct Game game;
  
  AEA_f32 near_z = 0.1f;
  AEA_f32 far_z = 100.0f;
  AEA_f32 aspect = AEA_GetAspectRatio(platform);
  AEA_f32 fov = glm_rad(45.0f);
  
  glm_perspective(fov, aspect, near_z, far_z, game.mat_projection);
  
  camera_init(&game.camera, (vec3){0.0f, 1.0f, 0.0f}, (vec3){0.0f, 1.0f, 0.0f});
  camera_recalculate_vectors(&game.camera);
  camera_get_view_matrix(&game.camera, game.mat_view);
  
  AEA_CaptureMouse(platform);
  
  // create a ground plane mesh
  struct AEA_Mesh ground_plane_mesh;
  ground_plane_mesh.vertex_count = 4;
  ground_plane_mesh.vertices = (struct AEA_Vertex *) malloc(sizeof(struct AEA_Vertex) * ground_plane_mesh.vertex_count);
  
  AEA_f32 vertex_positions[] = {
    -10.0f, 0.0f, -10.0f,
    10.0f, 0.0f, -10.0f,
    10.0f, 0.0f, 10.0f,
    -10.0f, 0.0f, 10.0f,
  };
  
  AEA_f32 vertex_normals[] = {
    0.0f, 1.0f, 0.0f,
    0.0f, 1.0f, 0.0f,
    0.0f, 1.0f, 0.0f,
    0.0f, 1.0f, 0.0f,
  };
  
  AEA_f32 vertex_colors[] = {
    1.0f, 0.0f, 0.0f, 1.0f,
    0.0f, 1.0f, 0.0f, 1.0f,
    0.0f, 0.0f, 1.0f, 1.0f,
    1.0f, 1.0f, 1.0f, 1.0f,

//    0.42f, 0.639f, 0.459f, 1.0f,
//    0.42f, 0.639f, 0.459f, 1.0f,
//    0.42f, 0.639f, 0.459f, 1.0f,
//    0.42f, 0.639f, 0.459f, 1.0f,
  };
  
  AEA_f32 vertex_uvs[] = {
    0.0f, 1.0f,
    1.0f, 1.0f,
    1.0f, 0.0f,
    0.0f, 0.0f,
  };
  
  for (int i = 0; i < 4; i++) {
    struct AEA_Vertex *vtx = &ground_plane_mesh.vertices[i];
    vtx->position[0] = vertex_positions[i * 3];
    vtx->position[1] = vertex_positions[i * 3 + 1];
    vtx->position[2] = vertex_positions[i * 3 + 2];
    vtx->normal[0] = vertex_normals[i * 3];
    vtx->normal[1] = vertex_normals[i * 3 + 1];
    vtx->normal[2] = vertex_normals[i * 3 + 2];
    vtx->color[0] = vertex_colors[i * 4];
    vtx->color[1] = vertex_colors[i * 4 + 1];
    vtx->color[2] = vertex_colors[i * 4 + 2];
    vtx->color[3] = vertex_colors[i * 4 + 3];
    vtx->uv[0] = vertex_uvs[i * 2] * 10;
    vtx->uv[1] = vertex_uvs[i * 2 + 1] * 10;
  }
  
  ground_plane_mesh.index_count = 6;
  ground_plane_mesh.indices = (AEA_u32 *) malloc(sizeof(AEA_u32) * ground_plane_mesh.index_count);
  ground_plane_mesh.indices[0] = 0;
  ground_plane_mesh.indices[1] = 1;
  ground_plane_mesh.indices[2] = 2;
  ground_plane_mesh.indices[3] = 0;
  ground_plane_mesh.indices[4] = 2;
  ground_plane_mesh.indices[5] = 3;
  
  // create the gl buffers for ground plane mesh (using uppercase naming for opengl convention)
  AEA_u32 VAO, VBO, EBO;
  glGenVertexArrays(1, &VAO);
  AEA_CheckGLError();
  glGenBuffers(1, &VBO);
  AEA_CheckGLError();
  glGenBuffers(1, &EBO);
  AEA_CheckGLError();
  
  glBindVertexArray(VAO);
  AEA_CheckGLError();
  glBindBuffer(GL_ARRAY_BUFFER, VBO);
  AEA_CheckGLError();
  glBufferData(GL_ARRAY_BUFFER, sizeof(struct AEA_Vertex) * ground_plane_mesh.vertex_count, ground_plane_mesh.vertices, GL_STATIC_DRAW);
  AEA_CheckGLError();
  
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
  AEA_CheckGLError();
  glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(AEA_u32) * ground_plane_mesh.index_count, ground_plane_mesh.indices, GL_STATIC_DRAW);
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
  
  // simple 2x2 checkerboard texture
  AEA_u8 ground_texture_data[16] = {
    0xFF, 0xFF, 0xFF, 0xFF, 0x00, 0x00, 0x00, 0xFF,
    0x00, 0x00, 0x00, 0xFF,0xFF, 0xFF, 0xFF, 0xFF,
  };
  AEA_u32 ground_texture;
  glGenTextures(1, &ground_texture);
  AEA_CheckGLError();
  
  glBindTexture(GL_TEXTURE_2D, ground_texture);
  AEA_CheckGLError();
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
  glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, 2, 2, 0, GL_RGBA, GL_UNSIGNED_BYTE, ground_texture_data);
  AEA_CheckGLError();
  glGenerateMipmap(GL_TEXTURE_2D);
  AEA_CheckGLError();
  
  struct AEA_Effect ground_effect;
  AEA_cstr ground_vert = "#version 330 core\n"
                         "layout (location = 0) in vec3 aPos;\n"
                         "layout (location = 1) in vec3 aNorm;\n"
                         "layout (location = 2) in vec4 aColor;\n"
                         "layout (location = 3) in vec2 aTexCoord;\n"
                         "\n"
                         "uniform mat4 mvp;\n"
                         "\n"
                         "out vec4 ourColor;\n"
                         "out vec2 TexCoord;\n"
                         "\n"
                         "void main()\n"
                         "{\n"
                         "  gl_Position = mvp * vec4(aPos, 1.0);\n"
                         "  ourColor = aColor;\n"
                         "  TexCoord = vec2(aTexCoord.x, aTexCoord.y);\n"
                         "}";
  
  AEA_cstr ground_frag = "#version 330 core\n"
                         "\n"
                         "out vec4 FragColor;\n"
                         "\n"
                         "in vec4 ourColor;\n"
                         "in vec2 TexCoord;\n"
                         "\n"
                         "uniform sampler2D texture1;\n"
                         "\n"
                         "void main()\n"
                         "{\n"
                         "  FragColor = ourColor * texture(texture1, TexCoord);\n"
                         "}";
  AEA_InitEffect(&ground_effect, ground_vert, ground_frag);
  AEA_CheckGLError();
  
  AEA_UseEffect(&ground_effect);
  AEA_CheckGLError();
  
  AEA_SetEffectUniformInt(&ground_effect, "texture1", 0);
  AEA_CheckGLError();
  
  mat4 ground_mat_model;
  glm_mat4_identity(ground_mat_model);
  
  mat4 mat_mvp;
  // Model * View * Projection
  glm_mat4_mul(game.mat_projection, game.mat_view, mat_mvp);
  glm_mat4_mul(mat_mvp, ground_mat_model, mat_mvp);
  
  AEA_SetEffectMVP(&ground_effect, "mvp", mat_mvp);
  AEA_CheckGLError();
  
  while (AEA_IsWindowOpen(platform)) {
    if (AEA_GetKeyState(platform, AEA_KEY_ESC))
    {
      printf("close!\n");
      AEA_CloseWindow(platform);
    }
    
    AEA_f64 delta_time = AEA_GetDeltaTime(platform);
    
    // update here using delta_time to scale motion
    AEA_s32 fwd = AEA_GetKeyState(platform, AEA_KEY_W) || AEA_GetKeyState(platform, AEA_KEY_UP);
    AEA_s32 bck = AEA_GetKeyState(platform, AEA_KEY_S) || AEA_GetKeyState(platform, AEA_KEY_DOWN);
    AEA_s32 lft = AEA_GetKeyState(platform, AEA_KEY_A) || AEA_GetKeyState(platform, AEA_KEY_LEFT);
    AEA_s32 rgt = AEA_GetKeyState(platform, AEA_KEY_D) || AEA_GetKeyState(platform, AEA_KEY_RIGHT);
    
    if (fwd)
    {
      camera_movement(&game.camera, 0, -1, (AEA_f32)delta_time);
    }
    else if (bck)
    {
      camera_movement(&game.camera, 0, 1, (AEA_f32)delta_time);
    }
    
    if (lft)
    {
      camera_movement(&game.camera, -1, 0, (AEA_f32)delta_time);
    }
    else if (rgt)
    {
      camera_movement(&game.camera, 1, 0, (AEA_f32)delta_time);
    }
    
    AEA_f32 mouse_dx, mouse_dy, mouse_wheel;
    AEA_GetMouseMotion(platform, &mouse_dx, &mouse_dy);
    AEA_GetMouseWheel(platform, &mouse_wheel);
    
    camera_rotation(&game.camera, mouse_dx, mouse_dy);
    camera_zoom(&game.camera, mouse_wheel);
    camera_recalculate_vectors(&game.camera);
    camera_get_view_matrix(&game.camera, game.mat_view);
    
    AEA_RenderBegin(platform);
    
    // draw here
    glBindVertexArray(VAO);
    
    AEA_UseEffect(&ground_effect);
    
    // Model * View * Projection
    glm_perspective(glm_rad(game.camera.zoom), aspect, near_z, far_z, game.mat_projection);
    glm_mat4_mul(game.mat_projection, game.mat_view, mat_mvp);
    glm_mat4_mul(mat_mvp, ground_mat_model, mat_mvp);
    
    AEA_SetEffectMVP(&ground_effect, "mvp", mat_mvp);
    
    glDrawElements(GL_TRIANGLES, (GLsizei)ground_plane_mesh.index_count, GL_UNSIGNED_INT, 0);
    
    AEA_RenderEnd(platform);
  }
  
  // do cleanup here
  
  AEA_ReleaseMouse(platform);
  
  glDeleteTextures(1, &ground_texture);
  
  glDeleteVertexArrays(1, &VAO);
  glDeleteBuffers(1, &VBO);
  glDeleteBuffers(1, &EBO);
  
  if (ground_plane_mesh.indices)
  {
    free(ground_plane_mesh.indices);
  }
  
  if (ground_plane_mesh.vertices)
  {
    free(ground_plane_mesh.vertices);
  }
  
  AEA_DestroyPlatform(platform);
  platform = NULL;
  
  return 0;
}
