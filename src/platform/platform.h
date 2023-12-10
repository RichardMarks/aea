//
// Created by Richard Marks on 12/10/23.
//

#ifndef AEA_PLATFORM_H
#define AEA_PLATFORM_H

#include "internal/platform_types.h"

struct AEA_PlatformInternal;

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

#define AEA_KEY_ESC 256

#endif //AEA_PLATFORM_H
