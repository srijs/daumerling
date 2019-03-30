#ifndef THUMBNAIL_H
#define THUMBNAIL_H

#include <stdio.h>

extern int thumbnail_width;
extern int thumbnail_height;

void thumbnail_process(char *format, int max_width, int max_height);

#endif
