#include <animations.h>

uint8_t update_model(float delta, ModelInstance* instance)
{
    switch (instance->modelFormat)
    {
        case MTYPE_UMB:  return umb_update_model ((UMB*) instance->model, delta, instance);
        case MTYPE_FMB:  return fmb_update_model ((FMB*) instance->model, delta, instance);
        case MTYPE_FMB2: return fmb2_update_model((FMB2*)instance->model, delta, instance);
    }

    return 2;
}