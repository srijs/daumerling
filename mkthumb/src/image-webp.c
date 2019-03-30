#include <stdio.h>

#include <webp/encode.h>

#include "config.h"
#include "image.h"
#include "utils.h"

static int writer(const uint8_t *data, size_t data_size,
                  const WebPPicture *picture) {
  size_t n = fwrite(data, sizeof(uint8_t), data_size, stdout);
  if (n != data_size) {
    FAIL("failed to write output data");
  }
  return 1;
}

void image_write_webp(Image *image) {
  WebPConfig config;
  if (!WebPConfigPreset(&config, WEBP_PRESET_DEFAULT, IMAGE_QUALITY)) {
    FAIL("failed to initialize encoder config");
  }

  if (!WebPValidateConfig(&config)) {
    FAIL("encoder config validation failed");
  }

  WebPPicture picture;
  if (!WebPPictureInit(&picture)) {
    FAIL("failed to initialize encoder");
  }

  picture.width = image->width;
  picture.height = image->height;

  picture.use_argb = 1;
  picture.argb = (uint32_t *)image->bgra.buffer;
  picture.argb_stride = image->bgra.stride / 4;

  picture.writer = writer;

  if (!WebPEncode(&config, &picture)) {
    FAIL("failed to encode image");
  }
}
