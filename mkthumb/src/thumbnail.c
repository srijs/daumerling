#include <errno.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include <webp/decode.h>

#include "image.h"
#include "memory.h"
#include "thumbnail.h"
#include "utils.h"

#define READ_BUFFER_SIZE (16 * 1024)

static void decode_and_scale(Image *image, int max_width, int max_height) {
  uint8_t read_buffer[READ_BUFFER_SIZE];

  ssize_t n = read(STDIN_FILENO, read_buffer, READ_BUFFER_SIZE);
  if (n == -1) {
    FAIL("initial read failed: %s", strerror(errno));
  }

  if (!WebPGetInfo(read_buffer, n, &image->width, &image->height)) {
    FAIL("failed to validate input image");
  }

  fit_width_and_height(max_width, max_height, &image->width, &image->height);

  WebPDecoderConfig dec_config;
  if (!WebPInitDecoderConfig(&dec_config)) {
    FAIL("failed to initialize decoder config");
  }

  dec_config.options.use_scaling = 1;
  dec_config.options.scaled_width = image->width;
  dec_config.options.scaled_height = image->height;

  dec_config.output.colorspace = MODE_BGRA;

  WebPIDecoder *dec = WebPIDecode(NULL, 0, &dec_config);

  do {
    VP8StatusCode status = WebPIAppend(dec, read_buffer, n);
    if (status != VP8_STATUS_OK && status != VP8_STATUS_SUSPENDED) {
      FAIL("decode failed with status code %d", status);
    }

    n = read(STDIN_FILENO, read_buffer, READ_BUFFER_SIZE);
    if (n == -1) {
      FAIL("read failed: %s", strerror(errno));
    }
  } while (n > 0);

  image->bgra.buffer = dec_config.output.u.RGBA.rgba;
  image->bgra.stride = dec_config.output.u.RGBA.stride;
}

void thumbnail_process(char *format, int max_width, int max_height) {
  Image image;
  decode_and_scale(&image, max_width, max_height);

  if (strcmp(format, "png") == 0) {
    image_write_png(&image);
  } else if (strcmp(format, "jpeg") == 0) {
    image_write_jpeg(&image);
  } else if (strcmp(format, "webp") == 0) {
    image_write_webp(&image);
  } else {
    FAIL("unknown output format");
  }
}
