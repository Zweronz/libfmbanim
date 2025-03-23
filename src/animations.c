#include <animations.h>

void delete_frames(Frame* frames)
{
    // technically this has no reason to exist. oh well.
    free(frames);
}