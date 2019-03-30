#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include "config.h"
#include "sandbox.h"
#include "thumbnail.h"
#include "utils.h"

int main(int argc, char **argv) {
  sandbox_init();

  int opt;

  char *format = NULL;
  int max_width = 128;
  int max_height = 128;

  while ((opt = getopt(argc, argv, "f:w:h:")) != -1) {
    switch (opt) {
      case 'f':
        format = optarg;
      case 'w':
        max_width = atoi(optarg);
        break;
      case 'h':
        max_height = atoi(optarg);
        break;
      default:
        FAIL("usage: %s -f <format> -w <width> -h <height>\n", argv[0]);
    }
  }

  if (format == NULL) {
    FAIL("format argument not specified");
  }

  thumbnail_process(format, max_width, max_height);

  return 0;
}
