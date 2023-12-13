//
// Created by Richard Marks on 12/12/23.
//

#ifndef AEA_EFFECT_H
#define AEA_EFFECT_H

#include "internal/platform_internal.h"
#include <cglm/cglm.h>

struct AEA_Effect {
  AEA_u32 program_id;
};

extern struct AEA_Effect *AEA_CreateDefaultEffect();
extern struct AEA_Effect *AEA_CreateEffect(AEA_cstr vert_source, AEA_cstr frag_source);
extern struct AEA_Effect *AEA_CreateEffectFromFiles(AEA_cstr vert_filename, AEA_cstr frag_filename);

extern AEA_void AEA_InitDefaultEffect(struct AEA_Effect *effect);
extern AEA_void AEA_InitEffect(struct AEA_Effect *effect, AEA_cstr vert_source, AEA_cstr frag_source);
extern AEA_void AEA_InitEffectFromFiles(struct AEA_Effect *effect, AEA_cstr vert_filename, AEA_cstr frag_filename);

extern AEA_void AEA_DestroyEffect(struct AEA_Effect *effect);

extern AEA_void AEA_UseEffect(struct AEA_Effect *effect);

extern AEA_void AEA_SetEffectMVP(struct AEA_Effect *effect, AEA_cstr name, mat4 mat_mvp);

extern AEA_void AEA_SetEffectUniformInt(struct AEA_Effect *effect, AEA_cstr name, AEA_s32 value);
extern AEA_void AEA_SetEffectUniformFloat(struct AEA_Effect *effect, AEA_cstr name, AEA_f32 value);
extern AEA_void AEA_SetEffectUniformBool(struct AEA_Effect *effect, AEA_cstr name, AEA_s32 value);
extern AEA_void AEA_SetEffectUniformVec3(struct AEA_Effect *effect, AEA_cstr name, vec3 value);
extern AEA_void AEA_SetEffectUniformVec4(struct AEA_Effect *effect, AEA_cstr name, vec4 value);
extern AEA_void AEA_SetEffectUniformVec4Components(struct AEA_Effect *effect, AEA_cstr name, AEA_f32 x, AEA_f32 y, AEA_f32 z, AEA_f32 w);
extern AEA_void AEA_SetEffectUniformMat4(struct AEA_Effect *effect, AEA_cstr name, mat4 value);

#endif //AEA_EFFECT_H
