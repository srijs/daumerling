#ifndef UTILS_H
#define UTILS_H

#include <error.h>
#include <stdint.h>

#define FAIL(format, ...) error(1, 0, format, ##__VA_ARGS__)

#define MIN(a, b) (((a) < (b)) ? (a) : (b))

// Round up `size` to a multiple of `align`, where `align`
// is a power of two.
static inline size_t aligned_size(size_t size, size_t align) {
  return (size + align - 1) & -align;
}

// Convert a row of BGRA pixels to RGB, blending with a black background.
static inline void row_bgra_to_rgb(const uint8_t *bgra_row, int width,
                                   uint8_t *rgb_row) {
  for (int i = 0; i < width; i++) {
    float alpha = (float)bgra_row[i * 4 + 3] / 255.0;
    rgb_row[i * 3 + 0] = bgra_row[i * 4 + 2] * alpha;
    rgb_row[i * 3 + 1] = bgra_row[i * 4 + 1] * alpha;
    rgb_row[i * 3 + 2] = bgra_row[i * 4 + 0] * alpha;
  }
}

static inline void fit_width_and_height(int max_width, int max_height,
                                        int *width, int *height) {
  if (*width > max_width) {
    *height = ((float)max_width / (float)*width) * (float)*height;
    *width = max_width;
  }
  if (*height > max_height) {
    *width = ((float)max_height / (float)*height) * (float)*width;
    *height = max_height;
  }
}

#endif
