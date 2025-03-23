#pragma once

#include <malloc.h>
#include <string.h>

#define LERP(a, b, t) (a + t * (b - a))

#ifdef _WIN32
    #define EXPORT __declspec(dllexport)
#else
    #define EXPORT __attribute__((visibility("default")))
#endif