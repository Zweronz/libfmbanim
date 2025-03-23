#pragma once

#include <libfmb.h>
#include <common.h>

#define MODEL_TYPE_NONE 0

typedef enum ModelState
{
    MSTATE_DEFAULT, // locks model to first keyframe
    MSTATE_PAUSED,  // locks model in-place
    MSTATE_PLAYING, // unlocks model animation
    MSTATE_LOOPING  // unlocks and loops model animation
} ModelState;

typedef struct Frame
{
    uint16_t index, length;
} Frame;

typedef struct ModelInstance
{
    Vec3** vertices, **normals;

    void* model; // pointer to the libfmb model

    uint8_t modelFormat; // denotes the format of this instance (umb, fmb, fmb2, etc)

    ModelState state;

    uint16_t currentKeyframe, nextKeyframe;

    float frameProgress;

    uint16_t rangeLow, rangeHigh; // the keyframe range of this model

    Frame* frameTable; // stored elsewhere, loaded in sort_frames. waste of memory to store it in all instances
} ModelInstance;

// 0 - ran successfully, do not change model
// 1 - ran successfully, change model
// 2 - update failed, do not change model
EXPORT uint8_t update_model(float delta, ModelInstance* instance);
EXPORT void delete_model(ModelInstance* instance);