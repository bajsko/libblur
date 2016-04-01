//
//  libblur.h
//  libblur
//
//  Created by Klas Henriksson on 2016-03-27.
//  Copyright © 2016 bajsko. All rights reserved.
//

#ifndef libblur_h
#define libblur_h

#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <stdlib.h>
#include <math.h>

typedef struct bimage_t
{
    int32_t width;
    int32_t height;
    uint32_t* pixels;
} bimage_t;

typedef struct bkernel_t
{
    int32_t m;
    int32_t n;
    float* underlying_kernel;
} bkernel_t;

typedef struct bcolor_t
{
    uint32_t r;
    uint32_t g;
    uint32_t b;
    uint32_t a;
} bcolor_t;

bimage_t* bcreate_image(int32_t width, int32_t height, uint32_t* pixels);
bkernel_t* bcreate_kernel(int32_t m, int32_t n);

int brun_blur(bimage_t* img, bkernel_t* kernel);
int bfree_image(bimage_t* img);
int bfree_kernel(bkernel_t* kernel);

#endif /* libblur_h */
