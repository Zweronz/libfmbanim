#include <animations.h>

ModelInstance* umb_create_instance(UMB* umb)
{
    ModelInstance* instance = malloc(sizeof(ModelInstance));

    instance->model = umb;
    instance->modelFormat = MTYPE_UMB;

    instance->state = MSTATE_DEFAULT;

    instance->vertices = (Vec3**)calloc(umb->numObjects, sizeof(Vec3*));
    instance->normals =  (Vec3**)calloc(umb->numObjects, sizeof(Vec3*));
    
    for (int i = 0; i < umb->numObjects; i++)
    {
        instance->vertices[i] = (Vec3*)calloc(umb->objects[i].frames[0].numVertices, sizeof(Vec3));
        instance->normals[i]  = (Vec3*)calloc(umb->objects[i].frames[0].numVertices, sizeof(Vec3));
    }

    return instance;
}

Frame* umb_sort_frames(UMB* umb)
{
    // animation stuff has (most likely) been proven to remain consistent throughout objects
    // the umb format is rife with inefficient memory usage such as this
    Frame* frames = (Frame*)calloc(umb->objects[0].numKeyFrames, sizeof(Frame));

    for (int i = 0; i < umb->objects[0].numKeyFrames; i++)
    {
        frames[i].index = umb->objects[0].frames[i].number;

        if (i == umb->objects[0].numKeyFrames - 1)
        {
            frames[i].length = 1;
        }
        else
        {
            frames[i].length = umb->objects[i].frames[i + 1].number - umb->objects[i].frames[i].number;
        }
    }
}

void set_model_frame(UMB* umb, uint32_t index, ModelInstance* instance, uint16_t nextFrame)
{
    Vec3* vertices = instance->vertices[index];
    Vec3* normals = instance->normals[index];

    UMBVector3 vertex, normal,
    nextVertex, nextNormal;

    float progress = instance->frameProgress;
    uint16_t frame = instance->currentKeyframe;

    for (int i = 0; i < umb->objects[index].frames[0].numVertices; i++)
    {
        vertex = umb->objects[index].frames[frame].vertices[i];
        normal = umb->objects[index].frames[frame].normals[i];

        nextVertex = umb->objects[index].frames[nextFrame].vertices[i];
        nextNormal = umb->objects[index].frames[nextFrame].normals[i];

        vertices[i].x = LERP(vertex.x, nextVertex.x, progress);
        vertices[i].y = LERP(vertex.y, nextVertex.y, progress);
        vertices[i].z = LERP(vertex.z, nextVertex.z, progress);

        normals[i].x = LERP(normal.x, nextNormal.x, progress);
        normals[i].y = LERP(normal.y, nextNormal.y, progress);
        normals[i].z = LERP(normal.z, nextNormal.z, progress);
    }
}

uint16_t umb_get_next_frame(UMB* umb, ModelInstance* instance)
{
    if (instance->frameTable[instance->currentKeyframe].index >= instance->rangeHigh)
    {
        for (int i = 0; i < umb->objects[0].numKeyFrames; i++)
        {
            if (instance->frameTable[i].index == instance->rangeLow)
            {
                return instance->frameTable[i].index;
            }
        }
    }

    return instance->currentKeyframe + 1;
}

uint8_t umb_update_model(UMB* umb, float delta, ModelInstance* instance)
{
    switch (instance->state)
    {
        case MSTATE_PAUSED: return;

        case MSTATE_DEFAULT:
        {
            for (int i = 0; i < umb->numObjects; i++)
            {
                memcpy(instance->vertices[i], umb->objects[i].frames[0].vertices, umb->objects[i].frames[0].numVertices * sizeof(Vec3));
                memcpy(instance->normals[i],  umb->objects[i].frames[0].normals,  umb->objects[i].frames[0].numVertices * sizeof(Vec3));
            }

            return;
        }
    }

    instance->frameProgress += delta * (30.0f / instance->frameTable[instance->currentKeyframe].length);

    if (instance->frameProgress >= 1.0f)
    {
        if (instance->state != MSTATE_LOOPING)
        {
            if (instance->frameTable[instance->currentKeyframe].index >= instance->rangeHigh)
            {
                instance->state = MSTATE_PAUSED;
            }
        }

        instance->currentKeyframe = get_next_frame(umb, instance);
        instance->nextKeyframe = get_next_frame(umb, instance);

        instance->frameProgress -= 1.0f;
    }

    for (int i = 0; i < umb->numObjects; i++)
    {
        set_model_frame(umb, i, instance, instance->nextKeyframe);
    }
}