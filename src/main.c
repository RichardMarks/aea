#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

#include <cglm/cglm.h>

#include "platform/platform.h"
#include "platform/effect.h"
#include "platform/mesh.h"
#include "platform/mesh_primitives.h"

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

AEA_void InitSkyDomeVertexColorsUsingColorTable(struct AEA_Mesh* sky_dome_mesh, AEA_u32 num_segments, AEA_u32 num_rings,  const AEA_u8 *color_data, AEA_u32 width, AEA_u32 height);

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
  memset(&ground_plane_mesh, 0, sizeof(struct AEA_Mesh));
  AEA_InitPlaneMesh(&ground_plane_mesh, 100.0f, 100.0f);
  
  // scale the uvs so that there is one "checker" on the ground plane for each world space unit
  for (AEA_size i = 0; i < 4; i++) {
    ground_plane_mesh.vertices[i].uv[0] *= 50.0f;
    ground_plane_mesh.vertices[i].uv[1] *= 50.0f;
  }

  // simple 2x2 checkerboard texture
  AEA_u8 ground_texture_data[16] = {
    0xFF, 0xFF, 0xFF, 0xFF, 0x00, 0x00, 0x00, 0xFF,
    0x00, 0x00, 0x00, 0xFF,0xFF, 0xFF, 0xFF, 0xFF,
  };
  
  struct AEA_MeshRenderer *ground_renderer = AEA_CreateMeshRenderer();
  AEA_BindMeshMeshRendererMesh(ground_renderer, &ground_plane_mesh);
  AEA_BindMeshRendererTexture(ground_renderer, 2, 2, ground_texture_data);
  
  // create a sky dome mesh
  struct AEA_Mesh sky_dome_mesh;
  memset(&sky_dome_mesh, 0, sizeof(struct AEA_Mesh));
  AEA_InitHemisphereMesh2(&sky_dome_mesh, 50.0f, 15, 7);
  
  // load color table for sky
  // update sky dome vertex colors using color table
  {
    int width, height, components;
    AEA_u8 *color_data = stbi_load("sky.png", &width, &height, &components, 4);
    
    InitSkyDomeVertexColorsUsingColorTable(&sky_dome_mesh, 15, 7, color_data, width, height);
    
    stbi_image_free(color_data);
  }

//  AEA_WriteMeshToFile(&sky_dome_mesh, "sky_dome2.obj");
  
  struct AEA_MeshRenderer *sky_dome_renderer = AEA_CreateMeshRenderer();
  sky_dome_renderer->flags |= AEA_MESH_RENDERER_STRIP;
  AEA_BindMeshMeshRendererMesh(sky_dome_renderer, &sky_dome_mesh);
  
  AEA_u8 sky_texture_data[16] = {
    0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
    0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
  };

  AEA_BindMeshRendererTexture(sky_dome_renderer, 2, 2, sky_texture_data);
  
  
  
  
  struct AEA_Effect ground_effect;
  struct AEA_Effect sky_effect;
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
  
  AEA_InitEffect(&sky_effect, ground_vert, ground_frag);
  AEA_CheckGLError();
  
  AEA_UseEffect(&sky_effect);
  AEA_CheckGLError();
  
  AEA_SetEffectUniformInt(&sky_effect, "texture1", 0);
  AEA_CheckGLError();
  
  mat4 ground_mat_model;
  glm_mat4_identity(ground_mat_model);
  
  mat4 mat_mvp;
  // Model * View * Projection
  glm_mat4_mul(game.mat_projection, game.mat_view, mat_mvp);
  glm_mat4_mul(mat_mvp, ground_mat_model, mat_mvp);
  
  AEA_SetEffectMVP(&ground_effect, "mvp", mat_mvp);
  AEA_CheckGLError();
  
  AEA_SetEffectMVP(&sky_effect, "mvp", mat_mvp);
  AEA_CheckGLError();
  
  fprintf(stderr, "about to start main loop\n");
  
//  glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
  
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
    
    // Model * View * Projection
    glm_perspective(glm_rad(game.camera.zoom), aspect, near_z, far_z, game.mat_projection);
    glm_mat4_mul(game.mat_projection, game.mat_view, mat_mvp);
    glm_mat4_mul(mat_mvp, ground_mat_model, mat_mvp);
    
    AEA_DrawMeshRenderer(sky_dome_renderer, &sky_effect, mat_mvp);
    
    AEA_DrawMeshRenderer(ground_renderer, &ground_effect, mat_mvp);

    AEA_RenderEnd(platform);
  }
  
  // do cleanup here
  
  AEA_ReleaseMouse(platform);
  
  glBindVertexArray(0);
  
  AEA_DestroyMesh(&sky_dome_mesh);
  AEA_DestroyMesh(&ground_plane_mesh);
  
  AEA_DestroyMeshRenderer(sky_dome_renderer);
  AEA_DestroyMeshRenderer(ground_renderer);

  AEA_DestroyPlatform(platform);
  platform = NULL;
  
  return 0;
}

AEA_void GetRGBAf32(const AEA_u8 *color_data, vec4 out_rgba)
{
  out_rgba[0] = (AEA_f32)color_data[0] / 255.0f;
  out_rgba[1] = (AEA_f32)color_data[1] / 255.0f;
  out_rgba[2] = (AEA_f32)color_data[2] / 255.0f;
  out_rgba[3] = (AEA_f32)color_data[3] / 255.0f;
}

AEA_void InitSkyDomeVertexColorsUsingColorTable(struct AEA_Mesh* sky_dome_mesh, AEA_u32 num_segments, AEA_u32 num_rings,  const AEA_u8 *color_data, AEA_u32 width, AEA_u32 height)
{
  AEA_f32 time_of_day = 7.0f;
  AEA_f32 pi_over_2 = (AEA_f32)M_PI_2;
  AEA_f32 ring_angle = pi_over_2 / (AEA_f32)num_rings;
  AEA_f32 segment_angle = (AEA_f32) (M_PI * 2.0) / (AEA_f32)num_segments;
  
  AEA_size vertex_index = 0;
  for (AEA_u32 j = 0; j <= num_rings; j++)
  {
    for (AEA_u32 i = 0; i <= num_segments; i++)
    {
      // latitude coordinate in the dome
      AEA_f32 theta = pi_over_2 - (ring_angle * (AEA_f32)j);
      // longitude coordinate in the dome
      AEA_f32 phi = segment_angle * (AEA_f32)i;
      
      AEA_f32 xf32 = time_of_day / 24.0f;
      AEA_f32 yf32 = theta / pi_over_2;
      xf32 *= (AEA_f32)width;
      yf32 *= (AEA_f32)height;
      
      AEA_s32 current_color_x = (AEA_s32)xf32;
      AEA_s32 current_color_y = (AEA_s32)yf32;
      if (current_color_y >= height) {
        current_color_y = (AEA_s32)(height - 1);
      }
      AEA_s32 next_color_x = (current_color_x >= width - 1 ? current_color_x : current_color_x + 1);
      AEA_s32 next_color_y = (current_color_y >= height - 1 ? current_color_y : current_color_y + 1);
      
      vec4 c00;
      vec4 c01;
      vec4 c11;
      vec4 c10;
      
      GetRGBAf32(color_data + (current_color_x + (current_color_y * width)), c00);
      GetRGBAf32(color_data + (next_color_x + (current_color_y * width)), c01);
      GetRGBAf32(color_data + (next_color_x + (next_color_y * width)), c11);
      GetRGBAf32(color_data + (current_color_x + (next_color_y * width)), c10);
      
      AEA_f32 h = xf32 - (AEA_f32)current_color_x;
      AEA_f32 v = yf32 - (AEA_f32)current_color_y;
      
      // calculate the horizon color
      vec4 h0 = {
        c00[0] * (1.0f - h) + c01[0] * h,
        c00[1] * (1.0f - h) + c01[1] * h,
        c00[2] * (1.0f - h) + c01[2] * h,
        c00[3] * (1.0f - h) + c01[3] * h,
      };
      
      vec4 h1 = {
        c10[0] * (1.0f - h) + c11[0] * h,
        c10[1] * (1.0f - h) + c11[1] * h,
        c10[2] * (1.0f - h) + c11[2] * h,
        c10[3] * (1.0f - h) + c11[3] * h,
      };
      
      // interpolate to find the final color of the vertex
      vec4 final = {
        h0[0] * (1.0f - v) + h1[0] * v,
        h0[1] * (1.0f - v) + h1[1] * v,
        h0[2] * (1.0f - v) + h1[2] * v,
        h0[3] * (1.0f - v) + h1[3] * v,
      };
      
      if (vertex_index >= sky_dome_mesh->vertex_count)
      {
        fprintf(stderr, "VERTEX INDEX OUT OF BOUNDS j=%d i=%d vertex index %lu is > %lu\n", j, i, vertex_index, sky_dome_mesh->vertex_count - 1);
        break;
      }
      
      struct AEA_Vertex *vtx = &sky_dome_mesh->vertices[vertex_index];
      
      // update the vertex color using the final color
      glm_vec4_copy(final, vtx->color);
      
      vertex_index++;
    }
  }
}