//
// Created by Richard Marks on 12/10/23.
//

#include <stdio.h>
#include <stdlib.h>

#include "platform.h"
#include "internal/platform_internal.h"
#include "shader.h"

#define STR(x) #x
static AEA_cstr AEA_default_vertex_shader = "#version 330 core\nlayout (location = 0) in vec3 aPos;\nvoid main() { gl_Position = vec4(aPos, 1.0); }";

static AEA_cstr AEA_default_fragment_shader = "#version 330 core\nout vec4 FragColor;\nvoid main() { FragColor = vec4(1.0, 1.0, 1.0, 1.0); }";

static AEA_u32 AEA_default_shader_program;


static AEA_void AEA_HandleWindowResized(GLFWwindow *window, AEA_s32 w, AEA_s32 h)
{
  glViewport(0, 0, w, h);
}

static struct AEA_PlatformInternal* AEA_platform_internal_instance  = NULL;
static AEA_s32 AEA_first_mouse = 1;
static AEA_void AEA_HandleMouseMotion(GLFWwindow *window, AEA_f64 x, AEA_f64 y)
{
  if (!AEA_platform_internal_instance)
  {
    return;
  }
  AEA_f32 x_pos = (AEA_f32)x;
  AEA_f32 y_pos = (AEA_f32)y;
  if (AEA_first_mouse)
  {
    AEA_platform_internal_instance->mouse_prev_x = x_pos;
    AEA_platform_internal_instance->mouse_prev_y = y_pos;
    AEA_first_mouse = 0;
  }
  
  AEA_platform_internal_instance->mouse_dx = x_pos - AEA_platform_internal_instance->mouse_prev_x;
  AEA_platform_internal_instance->mouse_dy = AEA_platform_internal_instance->mouse_prev_y - y_pos;
  
  AEA_platform_internal_instance->mouse_prev_x = x_pos;
  AEA_platform_internal_instance->mouse_prev_y = y_pos;
}

static AEA_void AEA_HandleMouseWheel(GLFWwindow *window, AEA_f64 x, AEA_f64 y)
{
  if (!AEA_platform_internal_instance)
  {
    return;
  }
  AEA_platform_internal_instance->mouse_wheel = (AEA_f32)y;
}

struct AEA_Platform *AEA_CreatePlatform(struct AEA_CreatePlatformParams *create_params)
{
  struct AEA_Platform *platform = (struct AEA_Platform *)malloc(sizeof(struct AEA_Platform));
  
  if (!platform)
  {
    return NULL;
  }
  
  platform->_internal = (struct AEA_PlatformInternal *)(malloc(sizeof(struct AEA_PlatformInternal)));
  
  if (!platform->_internal)
  {
    free(platform);
    return NULL;
  }
  
  AEA_platform_internal_instance = platform->_internal;
  
  platform->_internal->window = NULL;
  
  glfwInit();
  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
  // macOS requirement
#ifdef __APPLE__
  glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif
  
  platform->_internal->window = glfwCreateWindow(create_params->window_width, create_params->window_height, create_params->window_title, NULL, NULL);
  
  if (!platform->_internal->window)
  {
    fprintf(stderr, "failed to create the window\n");
    free(platform->_internal);
    AEA_platform_internal_instance = NULL;
    free(platform);
    glfwTerminate();
    return NULL;
  }
  
  glfwMakeContextCurrent(platform->_internal->window);
  
//  AEA_CheckGLError();
  
  if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
  {
    fprintf(stderr, "failed to initialize GLAD\n");
    free(platform->_internal);
    AEA_platform_internal_instance = NULL;
    free(platform);
    glfwTerminate();
    return NULL;
  }
  
  glViewport(0, 0, create_params->window_width, create_params->window_height);
  glfwSetFramebufferSizeCallback(platform->_internal->window, AEA_HandleWindowResized);
  
  glfwSetCursorPosCallback(platform->_internal->window, AEA_HandleMouseMotion);
  glfwSetScrollCallback(platform->_internal->window, AEA_HandleMouseWheel);
#ifdef __APPLE__
  // an awful macOS hacky workaround to get proper rendering at start
  glfwSwapBuffers(platform->_internal->window);
  AEA_s32 window_position_x = 0;
  AEA_s32 window_position_y = 0;
  glfwGetWindowPos(platform->_internal->window, &window_position_x, &window_position_y);
  glfwSetWindowPos(platform->_internal->window, window_position_x++, window_position_y);
  glfwSetWindowPos(platform->_internal->window, window_position_x--, window_position_y);
  glfwSwapBuffers(platform->_internal->window);
#endif
  
  AEA_CheckGLError();
  
  struct AEA_BuildShaderStatus status;
  AEA_default_shader_program = AEA_BuildShaderProgram(AEA_default_vertex_shader, AEA_default_fragment_shader, &status);
  if (!status.success)
  {
    fprintf(stderr, "failed to build default shader program\n");
    free(platform->_internal);
    AEA_platform_internal_instance = NULL;
    free(platform);
    glfwTerminate();
    return NULL;
  }
  
  glEnable(GL_DEPTH_TEST);
  AEA_CheckGLError();
  
  glUseProgram(AEA_default_shader_program);
  AEA_CheckGLError();
  
  return platform;
}

AEA_void AEA_DestroyPlatform(struct AEA_Platform *platform)
{
  if (platform)
  {
    if (platform->_internal) {
      free(platform->_internal);
      AEA_platform_internal_instance = NULL;
      glfwTerminate();
    }
    free(platform);
  }
}

AEA_s32 AEA_IsWindowOpen(struct AEA_Platform *platform)
{
  AEA_s32 should_close = glfwWindowShouldClose(platform->_internal->window);
  
  if (!should_close)
  {
    // fix for the drifting mouse look
    platform->_internal->mouse_dx = 0;
    platform->_internal->mouse_dy = 0;
    
    // fix sliding zoom
    platform->_internal->mouse_wheel = 0;
    
    glfwPollEvents();
    AEA_f64 current_time = glfwGetTime();
    platform->_internal->delta_time = (current_time - platform->_internal->last_time);
    platform->_internal->elapsed_time += platform->_internal->delta_time;
    platform->_internal->last_time = current_time;
  }
  
  return !should_close;
}

AEA_void AEA_CloseWindow(struct AEA_Platform *platform)
{
  glfwSetWindowShouldClose(platform->_internal->window, GLFW_TRUE);
}

AEA_s32 AEA_GetKeyState(struct AEA_Platform *platform, AEA_s32 key)
{
  return glfwGetKey(platform->_internal->window, key);
}

AEA_f64 AEA_GetDeltaTime(struct AEA_Platform *platform)
{
  return platform->_internal->delta_time;
}

AEA_void AEA_RenderBegin(struct AEA_Platform *platform)
{
  glClearColor(0.2f, 0.4f, 0.6f, 1.0f);
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

AEA_void AEA_RenderEnd(struct AEA_Platform *platform)
{
  glfwSwapBuffers(platform->_internal->window);
}

AEA_f32 AEA_GetAspectRatio(struct AEA_Platform *platform)
{
  AEA_s32 width, height;
  glfwGetFramebufferSize(platform->_internal->window, &width, &height);
  AEA_f32 aspect_ratio = ((AEA_f32)width / (AEA_f32)height);
  return aspect_ratio;
}

AEA_void AEA_GetMousePosition(struct AEA_Platform *platform, AEA_f32 *out_x, AEA_f32 *out_y)
{
  *out_x = platform->_internal->mouse_x;
  *out_y = platform->_internal->mouse_y;
}

AEA_void AEA_GetMouseMotion(struct AEA_Platform *platform, AEA_f32 *out_dx, AEA_f32 *out_dy)
{
  *out_dx = platform->_internal->mouse_dx;
  *out_dy = platform->_internal->mouse_dy;
}

AEA_void AEA_GetMouseWheel(struct AEA_Platform *platform, AEA_f32 *out_wheel)
{
  *out_wheel = platform->_internal->mouse_wheel;
}

AEA_void AEA_CaptureMouse(struct AEA_Platform *platform)
{
  glfwSetInputMode(platform->_internal->window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
}

AEA_void AEA_ReleaseMouse(struct AEA_Platform *platform)
{
  glfwSetInputMode(platform->_internal->window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
}
