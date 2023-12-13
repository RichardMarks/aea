//
// Created by Richard Marks on 12/10/23.
//

#ifndef AEA_PLATFORM_H
#define AEA_PLATFORM_H

#include <stdio.h>
#include "internal/platform_internal.h"

struct AEA_Platform {
  struct AEA_PlatformInternal *_internal;
};

struct AEA_CreatePlatformParams {
  AEA_cstr window_title;
  AEA_s32 window_width;
  AEA_s32 window_height;
  AEA_s32 fullscreen;
};

extern struct AEA_Platform *AEA_CreatePlatform(struct AEA_CreatePlatformParams *create_params);
extern AEA_void AEA_DestroyPlatform(struct AEA_Platform *platform);

extern AEA_s32 AEA_IsWindowOpen(struct AEA_Platform *platform);
extern AEA_void AEA_CloseWindow(struct AEA_Platform *platform);

extern AEA_s32 AEA_GetKeyState(struct AEA_Platform *platform, AEA_s32 key);

extern AEA_f64 AEA_GetDeltaTime(struct AEA_Platform *platform);

extern AEA_void AEA_RenderBegin(struct AEA_Platform *platform);
extern AEA_void AEA_RenderEnd(struct AEA_Platform *platform);

extern AEA_f32 AEA_GetAspectRatio(struct AEA_Platform *platform);

extern AEA_void AEA_GetMousePosition(struct AEA_Platform *platform, AEA_f32 *out_x, AEA_f32 *out_y);
extern AEA_void AEA_GetMouseMotion(struct AEA_Platform *platform, AEA_f32 *out_dx, AEA_f32 *out_dy);
extern AEA_void AEA_GetMouseWheel(struct AEA_Platform *platform, AEA_f32 *out_wheel);

extern AEA_void AEA_CaptureMouse(struct AEA_Platform *platform);
extern AEA_void AEA_ReleaseMouse(struct AEA_Platform *platform);

#define AEA_KEY_ESC 256
#define AEA_KEY_W 87
#define AEA_KEY_S 83
#define AEA_KEY_A 65
#define AEA_KEY_D 68

#define AEA_KEY_UP 265
#define AEA_KEY_DOWN 264
#define AEA_KEY_LEFT 263
#define AEA_KEY_RIGHT 262

#define AEA_KEY_SPACE 32

static GLenum AEA_CheckGLErrorA(AEA_cstr file, AEA_s32 line)
{
  fprintf(stderr, "%s CheckGLError: %s (%d) ... ", glGetString(GL_VERSION), file, line);
  GLenum error_code;
  while ((error_code = glGetError()) != GL_NO_ERROR)
  {
    AEA_char error_message[256];
    switch (error_code)
    {
    case GL_INVALID_ENUM: snprintf(error_message, 256, "INVALID_ENUM"); break;
    case GL_INVALID_VALUE:                 snprintf(error_message, 256, "INVALID_VALUE"); break;
    case GL_INVALID_OPERATION:             snprintf(error_message, 256, "INVALID_OPERATION"); break;
    case GL_STACK_OVERFLOW:                snprintf(error_message, 256, "STACK_OVERFLOW"); break;
    case GL_STACK_UNDERFLOW:               snprintf(error_message, 256, "STACK_UNDERFLOW"); break;
    case GL_OUT_OF_MEMORY:                 snprintf(error_message, 256, "OUT_OF_MEMORY"); break;
    case GL_INVALID_FRAMEBUFFER_OPERATION: snprintf(error_message, 256, "INVALID_FRAMEBUFFER_OPERATION"); break;
    default: break;
    }
    fprintf(stderr, "GL ERROR: %8X %s %s (%d)\n", error_code, error_message, file, line);
  }
  if (!error_code)
  {
    fprintf(stderr, "NO ERROR\n");
  }
  return error_code;
}
#define AEA_CheckGLError() AEA_CheckGLErrorA(__FILE__, __LINE__)

#endif //AEA_PLATFORM_H
