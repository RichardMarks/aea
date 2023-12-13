//
// Created by Richard Marks on 12/11/23.
//

#include "game_camera.h"

AEA_void camera_init(struct GameCamera *camera, vec3 position, vec3 up) {
  glm_vec3_copy(position, camera->position);
  glm_vec3_copy(up, camera->world_up);
  camera->yaw = DEF_CAMERA_YAW;
  camera->pitch = DEF_CAMERA_PITCH;
  camera->mouse_sensitivity = DEF_CAMERA_SENSITIVITY;
  camera->move_speed = DEF_CAMERA_SPEED;
  camera->zoom = DEF_CAMERA_ZOOM;
  camera->front[0] = 0.0f;
  camera->front[1] = 0.0f;
  camera->front[2] = -1.0f;
}

AEA_void camera_get_view_matrix(struct GameCamera *camera, mat4 out_view)
{
  vec3 center;
  glm_vec3_add(camera->position, camera->front, center);
  glm_lookat(camera->position, center, camera->up, out_view);
}

AEA_void camera_recalculate_vectors(struct GameCamera *camera)
{
  vec3 tmp = {
    cosf(glm_rad(camera->yaw)) * cosf(glm_rad(camera->pitch)),
    sinf(glm_rad(camera->pitch)),
    sinf(glm_rad(camera->yaw)) * cosf(glm_rad(camera->pitch))
  };
  glm_vec3_norm(tmp);
  glm_vec3_copy(tmp, camera->front);
  
  glm_vec3_cross(camera->front, camera->world_up, tmp);
  glm_vec3_norm(tmp);
  glm_vec3_copy(tmp, camera->right);
  
  glm_vec3_cross(camera->right, camera->front, tmp);
  glm_vec3_norm(tmp);
  glm_vec3_copy(tmp, camera->up);
}

AEA_void camera_movement(struct GameCamera *camera, AEA_s32 dx, AEA_s32 dz, AEA_f32 delta_time)
{
  AEA_f32 velocity = camera->move_speed * delta_time;
  if (dz < 0)
  {
    // position += front * velocity
    glm_vec3_muladds(camera->front, velocity, camera->position);
  } else if (dz > 0) {
    // position += front * -velocity
    glm_vec3_muladds(camera->front, -velocity, camera->position);
  }
  if (dx < 0) {
    // position += right * -velocity
    glm_vec3_muladds(camera->right, -velocity, camera->position);
  } else if (dx > 0) {
    // position += right * velocity
    glm_vec3_muladds(camera->right, velocity, camera->position);
  }
}

AEA_void camera_rotation(struct GameCamera *camera, AEA_f32 dx, AEA_f32 dy)
{
  camera->yaw += (dx * camera->mouse_sensitivity);
  camera->pitch += (dy * camera->mouse_sensitivity);
  camera->pitch  = glm_clamp(camera->pitch, -89.0f, 89.0f);
}

AEA_void camera_zoom(struct GameCamera *camera, AEA_f32 factor)
{
  camera->zoom -= factor;
  camera->zoom = glm_clamp(camera->zoom, 1.0f, 45.0f);
}
