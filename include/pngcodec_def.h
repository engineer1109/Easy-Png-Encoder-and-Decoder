//
// Created by wjl on 2020/7/12.
//

#ifndef PNGCODEC_PNGCODEC_DEF_H
#define PNGCODEC_PNGCODEC_DEF_H
#include <fstream>
#include <iostream>
#include <stdint.h>
#include <string>
#include <vector>

#if defined(__GNUC__)
#ifdef LIGPNGCODEC_EXPORT_API
#define PNGCODEC_EXPORT __attribute__((visibility("default")))
#else
#define PNGCODEC_EXPORT
#endif
#elif defined(_MSC_VER)
#ifdef LIGPNGCODEC_EXPORT_API
#define PNGCODEC_EXPORT __declspec(dllexport)
#else
#define PNGCODEC_EXPORT
#endif
#else
#define PNGCODEC_EXPORT
#endif

typedef struct {
    uint8_t *data = nullptr;
    int size = 0;
    int offset = 0;
} PngSource;

#endif // PNGCODEC_PNGCODEC_DEF_H
