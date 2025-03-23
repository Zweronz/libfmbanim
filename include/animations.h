#pragma once

#include <libfmb.h>
#include <modelinstance.h>
#include <common.h>

#define MTYPE_UMB 1
#define MTYPE_FMB 2
#define MTYPE_FMB2 3

EXPORT ModelInstance* umb_create_instance (UMB*   umb);
EXPORT ModelInstance* fmb_create_instance (FMB*   fmb);
EXPORT ModelInstance* fmb2_create_instance(FMB2* fmb2);

EXPORT Frame* umb_sort_frames (UMB*   umb);
EXPORT Frame* fmb_sort_frames (FMB*   fmb);
EXPORT Frame* fmb2_sort_frames(FMB2* fmb2);

EXPORT uint16_t umb_get_next_frame(UMB* umb, ModelInstance* instance);

EXPORT void delete_frames(Frame* frames);

uint8_t umb_update_model (UMB*  umb,  float delta, ModelInstance* instance);
uint8_t fmb_update_model (FMB*  fmb,  float delta, ModelInstance* instance);
uint8_t fmb2_update_model(FMB2* fmb2, float delta, ModelInstance* instance);