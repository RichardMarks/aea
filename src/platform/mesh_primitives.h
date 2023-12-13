//
// Created by Richard Marks on 12/13/23.
//

#ifndef AEA_MESH_PRIMITIVES_H
#define AEA_MESH_PRIMITIVES_H

#include "mesh.h"

extern struct AEA_Mesh *AEA_CreatePlaneMesh(AEA_f32 width, AEA_f32 depth);

extern AEA_void AEA_InitPlaneMesh(struct AEA_Mesh *mesh, AEA_f32 width, AEA_f32 depth);

#endif //AEA_MESH_PRIMITIVES_H
