#include <png.h>

#include "image.h"
#include "utils.h"

#define IMAGE_PNG_BATCH_SIZE 16

void image_write_png(Image *image) {
  png_structp png_ptr =
      png_create_write_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);
  if (!png_ptr) {
    FAIL("failed to create png write struct");
  }

  png_infop info_ptr = png_create_info_struct(png_ptr);
  if (!info_ptr) {
    FAIL("failed to create png info struct");
  }

  if (setjmp(png_jmpbuf(png_ptr))) {
    FAIL("internal error in png encoder");
  }

  png_init_io(png_ptr, stdout);

  png_set_IHDR(png_ptr, info_ptr, image->width, image->height, 8,
               PNG_COLOR_TYPE_RGBA, PNG_INTERLACE_NONE,
               PNG_COMPRESSION_TYPE_DEFAULT, PNG_FILTER_TYPE_DEFAULT);

  png_set_bgr(png_ptr);

  png_write_info(png_ptr, info_ptr);

  int stride = image->bgra.stride;
  png_byte *row_pointers[IMAGE_PNG_BATCH_SIZE];

  for (int i = 0; i < image->height; i += IMAGE_PNG_BATCH_SIZE) {
    int number_of_rows = MIN(image->height - i, IMAGE_PNG_BATCH_SIZE);
    for (int j = 0; j < number_of_rows; j++) {
      row_pointers[j] = &image->bgra.buffer[(i + j) * stride];
    }
    png_write_rows(png_ptr, row_pointers, number_of_rows);
  }

  png_write_end(png_ptr, info_ptr);
}
