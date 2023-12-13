//
// Created by Richard Marks on 12/11/23.
//

#ifndef AEA_GAME_CAMERA_H
#define AEA_GAME_CAMERA_H

#include "platform/platform.h"
#include <cglm/cglm.h>

//#define DEF_CAMERA_YAW (-90.0f)
#define DEF_CAMERA_YAW (0.0f)
#define DEF_CAMERA_PITCH 0.0f
#define DEF_CAMERA_SPEED 4.5f
#define DEF_CAMERA_SENSITIVITY 0.1f
#define DEF_CAMERA_ZOOM 45.0f

struct GameCamera
{
  CGLM_ALIGN(4) AEA_f32 yaw;
  CGLM_ALIGN(4) AEA_f32 pitch;
  CGLM_ALIGN(4) AEA_f32 move_speed;
  CGLM_ALIGN(4) AEA_f32 mouse_sensitivity;
  CGLM_ALIGN(4) AEA_f32 zoom;
  CGLM_ALIGN(16) vec3 position;
  CGLM_ALIGN(16) vec3 front;
  CGLM_ALIGN(16) vec3 up;
  CGLM_ALIGN(16) vec3 right;
  CGLM_ALIGN(16) vec3 world_up;
};

extern AEA_void camera_init(struct GameCamera *camera, vec3 position, vec3 up);

extern AEA_void camera_get_view_matrix(struct GameCamera *camera, mat4 out_view);

extern AEA_void camera_recalculate_vectors(struct GameCamera *camera);

extern AEA_void camera_movement(struct GameCamera *camera, AEA_s32 dx, AEA_s32 dz, AEA_f32 delta_time);

extern AEA_void camera_rotation(struct GameCamera *camera, AEA_f32 dx, AEA_f32 dy);

extern AEA_void camera_zoom(struct GameCamera *camera, AEA_f32 factor);

#endif //AEA_GAME_CAMERA_H
