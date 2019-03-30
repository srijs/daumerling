#ifndef IMAGE_H
#define IMAGE_H

#include <stdint.h>
#include <stdlib.h>
#include <unistd.h>

typedef struct {
  uint8_t* buffer;
  int stride;
} ImageDataBGRA;

typedef struct {
  int width;
  int height;
  ImageDataBGRA bgra;
} Image;

void image_write_png(Image* image);
void image_write_jpeg(Image* image);
void image_write_webp(Image* image);

#endif
