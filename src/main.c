#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "platform/platform.h"


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
 *
 * */


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
  
  while (AEA_IsWindowOpen(platform)) {
    if (AEA_GetKeyState(platform, AEA_KEY_ESC))
    {
      printf("close!\n");
      AEA_CloseWindow(platform);
    }
    
    AEA_f64 delta_time = AEA_GetDeltaTime(platform);
    
    // update here using delta_time to scale motion
    
    AEA_RenderBegin(platform);
    
    // draw here
    
    AEA_RenderEnd(platform);
  }
  
  // do cleanup here
  
  AEA_DestroyPlatform(platform);
  platform = NULL;
  
  return 0;
}
