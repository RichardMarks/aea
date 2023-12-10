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
};

#endif //AEA_PLATFORM_INTERNAL_H
