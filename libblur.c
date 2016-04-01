//
//  libblur.c
//  libblur
//
//  Created by Klas Henriksson on 2016-03-27.
//  Copyright © 2016 bajsko. All rights reserved.
//

#include "libblur.h"

bimage_t* bcreate_image(int32_t width, int32_t height, uint32_t* pixels)
{
    uint32_t* pixels_array = malloc(sizeof(uint32_t) * width * height);
    
    for(int i = 0; i < width * height; i++)
        pixels_array[i] = pixels[i];
    
    bimage_t* image = malloc(sizeof(bimage_t));
    image->pixels = pixels_array;
    image->width = width;
    image->height = height;
    
    printf("libblur: Created image with size: %dx%d\n", width, height);
    
    return image;
}

bkernel_t* bcreate_kernel(int32_t m, int32_t n)
{
    float* kernel = malloc(sizeof(float) * (m * 2) * (n * 2));
    
    memset(kernel, 1, m * n);
    
    bkernel_t* ret = malloc(sizeof(bkernel_t));
    ret->m = m;
    ret->n = n;
    ret->underlying_kernel = kernel;
    
    printf("libblur: Created kernel with size: %dx%d\n", m, n);
    
    return ret;
}

static uint32_t ConstructPixel(bcolor_t color)
{
    uint32_t ret = 0;
    ret |= (color.r << 24);
    ret |= (color.g << 16);
    ret |= (color.b << 8);
    ret |= 0;
    
    return ret;
}

static bcolor_t GetColorFromPixel(uint32_t pixel)
{
    bcolor_t ret;
    ret.r = ((pixel & 0xFF000000) >> 24);
    ret.g = ((pixel & 0xFF0000) >> 16);
    ret.b = ((pixel & 0xFF00) >> 8);
    ret.a = ((pixel & 0xFF));
    
    return ret;
}

static uint32_t GetPixel(int x, int y, bimage_t* img)
{
    
    if(x < 0) x = 0;
    if(y < 0) y = 0;
    if(x >= img->width) x = img->width - 1;
    if(y >= img->height) y = img->height - 1;
    
    return img->pixels[x + y * img->width];
}

int brun_blur(bimage_t* img, bkernel_t* kernel)
{
    for(int x = 0; x < img->width; x++)
    {
        for(int y = 0; y < img->height; y++)
        {
            uint32_t pixel = GetPixel(x, y, img);
            bcolor_t color;
            
            for(int i = -kernel->m; i < kernel->m; i++)
            {
                for(int j = -kernel->n; j < kernel->n; j++)
                {
                    uint32_t kpixel = GetPixel(x+i, y+j, img);
                    bcolor_t kcolor = GetColorFromPixel(kpixel);
                    
                    color.r += kcolor.r;
                    color.g += kcolor.g;
                    color.b += kcolor.b;
                }
            }
            
            color.r /= ((kernel->m * 2) * (kernel->n * 2));
            color.g /= ((kernel->m * 2) * (kernel->n * 2));
            color.b /= ((kernel->m * 2) * (kernel->n * 2));
            
            pixel = ConstructPixel(color);
            img->pixels[x + y * img->width] = pixel;
        }
    }
    
    printf("libblur: run complete!\n");
    
    return 0;
}

int bfree_image(bimage_t* img)
{
    free(img->pixels);
    free(img);
    return 0;
}

int bfree_kernel(bkernel_t* kernel)
{
    free(kernel);
    return 0;
}