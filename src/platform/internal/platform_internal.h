//
// Created by Richard Marks on 12/10/23.
//

#ifndef AEA_PLATFORM_INTERNAL_H
#define AEA_PLATFORM_INTERNAL_H

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "platform_types.h"

struct AEA_PlatformInternal {
  GLFWwindow *window;
  AEA_f64 last_time;
  AEA_f64 delta_time;
  AEA_f64 elapsed_time;
  
  AEA_f32 mouse_x;
  AEA_f32 mouse_y;
  AEA_f32 mouse_dx;
  AEA_f32 mouse_dy;
  AEA_f32 mouse_prev_x;
  AEA_f32 mouse_prev_y;
  AEA_s32 mouse_down;
  AEA_f32 mouse_wheel;
};

#endif //AEA_PLATFORM_INTERNAL_H
