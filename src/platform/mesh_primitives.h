//
// Created by Richard Marks on 12/13/23.
//

#ifndef AEA_MESH_PRIMITIVES_H
#define AEA_MESH_PRIMITIVES_H

#include "mesh.h"

extern struct AEA_Mesh *AEA_CreatePlaneMesh(AEA_f32 width, AEA_f32 depth);

extern struct AEA_Mesh *AEA_CreateHemisphereMesh(AEA_f32 radius, AEA_s32 num_segments, AEA_s32 num_rings);

extern AEA_void AEA_InitPlaneMesh(struct AEA_Mesh *mesh, AEA_f32 width, AEA_f32 depth);

extern AEA_void AEA_InitHemisphereMesh(struct AEA_Mesh *mesh, AEA_f32 radius, AEA_s32 num_segments, AEA_s32 num_rings);

#endif //AEA_MESH_PRIMITIVES_H
