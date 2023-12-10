//
// Created by Richard Marks on 12/10/23.
//

#include <stdio.h>
#include <stdlib.h>

#include "platform.h"
#include "internal/platform_internal.h"

#define STR(x) #x
static AEA_cstr AEA_default_vertex_shader = "#version 330 core layout (location 0) in vec3 aPos; void main() { gl_Position = vec4(aPos, 1.0); }";

static AEA_cstr AEA_default_fragment_shader = "#version 330 core out vec4 FragColor; void main() { FragColor = vec4(1.0, 1.0, 1.0, 1.0); }";

static AEA_u32 AEA_default_shader_program;


static AEA_void AEA_HandleWindowResized(GLFWwindow *window, AEA_s32 w, AEA_s32 h)
{
  glViewport(0, 0, w, h);
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
    free(platform);
    glfwTerminate();
    return NULL;
  }
  
  glfwMakeContextCurrent(platform->_internal->window);
  
  if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
  {
    fprintf(stderr, "failed to initialize GLAD\n");
    free(platform->_internal);
    free(platform);
    glfwTerminate();
    return NULL;
  }
  
  glViewport(0, 0, create_params->window_width, create_params->window_height);
  glfwSetFramebufferSizeCallback(platform->_internal->window, AEA_HandleWindowResized);
  
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
  
  AEA_u32 vs = glCreateShader(GL_VERTEX_SHADER);
  glShaderSource(vs, 1, &AEA_default_vertex_shader, NULL);
  glCompileShader(vs);
  
  AEA_u32 fs = glCreateShader(GL_FRAGMENT_SHADER);
  glShaderSource(fs, 1, &AEA_default_fragment_shader, NULL);
  glCompileShader(fs);
  
  AEA_default_shader_program = glCreateProgram();
  glAttachShader(AEA_default_shader_program, vs);
  glAttachShader(AEA_default_shader_program, fs);
  glLinkProgram(AEA_default_shader_program);
  
  glDeleteShader(vs);
  glDeleteShader(fs);
  
  
  glEnable(GL_DEPTH_TEST);
  
  glUseProgram(AEA_default_shader_program);
  
  return platform;
}

AEA_void AEA_DestroyPlatform(struct AEA_Platform *platform)
{
  if (platform)
  {
    if (platform->_internal) {
      free(platform->_internal);
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
    glfwPollEvents();
    AEA_f64 current_time = glfwGetTime();
    platform->_internal->delta_time = (current_time - platform->_internal->last_time);
    platform->_internal->elapsed_time += platform->_internal->delta_time;
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
