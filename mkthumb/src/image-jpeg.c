#include <stdio.h>

#include <jpeglib.h>

#include "config.h"
#include "image.h"
#include "memory.h"
#include "utils.h"

#define IMAGE_JPEG_BATCH_SIZE 16

void image_write_jpeg(Image *image) {
  struct jpeg_compress_struct cinfo;
  struct jpeg_error_mgr jerr;

  cinfo.err = jpeg_std_error(&jerr);
  jpeg_create_compress(&cinfo);

  jpeg_stdio_dest(&cinfo, stdout);

  cinfo.image_width = image->width;
  cinfo.image_height = image->height;
  cinfo.input_components = 3;
  cinfo.in_color_space = JCS_RGB;

  jpeg_set_defaults(&cinfo);
  jpeg_set_quality(&cinfo, IMAGE_QUALITY, TRUE);

  jpeg_start_compress(&cinfo, TRUE);

  int stride = image->bgra.stride;
  JSAMPROW row_pointers[IMAGE_JPEG_BATCH_SIZE];
  uint8_t *rows = malloc(3 * image->width * IMAGE_JPEG_BATCH_SIZE);

  for (int y = 0; y < IMAGE_JPEG_BATCH_SIZE; y++) {
    row_pointers[y] = &rows[image->width * 3 * y];
  }

  while (cinfo.next_scanline < cinfo.image_height) {
    int number_of_rows =
        MIN(image->height - cinfo.next_scanline, IMAGE_JPEG_BATCH_SIZE);
    for (int i = 0; i < number_of_rows; i++) {
      uint8_t *bgra_row =
          &image->bgra.buffer[(cinfo.next_scanline + i) * stride];
      row_bgra_to_rgb(bgra_row, image->width, row_pointers[i]);
    }
    jpeg_write_scanlines(&cinfo, row_pointers, number_of_rows);
  }

  jpeg_finish_compress(&cinfo);
}
