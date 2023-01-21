# include <stdio.h>
# include <stdlib.h>
# include <sys/stat.h>

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

static char ASCII_CHARS[11] = {'.', ',', ':', ';', '+', '*', '?', '%', 'S', '#', '@'};

int main(int ac, char **av) {
  int rc;
  int width;
  int height;
  int channels;
  size_t img_size;
  unsigned char *image, *p;
  int pexel = 0, lines = 0;
  int downscale_factor = 1;

  if (ac < 2)
  {
    fprintf(stderr, "Usage: %s \"Your image\" [scaledown factor (int)]\n", av[0]);
    return 1;
  }

  if (av[2]) {
    if (sscanf(av[2], "%d", &downscale_factor) != 1) {
      fprintf(stderr, "wrong scaledown factor %s, should be int\n", av[2]);
      return 1;	
    }
  }

  image = stbi_load(av[1], &width, &height, &channels, 0);
  if (!image)
  {
    fprintf(stderr, "Error: image loading fails\n");
    return 1;
  }

  // ... channels = # 8-bit components per pixel ...
  // ... replace the last argement with '0'..'4' to force that many components per pixel
  // ... but 'n' will always be the number that it would have been if you said
  img_size = (width * height * channels) - channels;

  for (p = image; p < image + img_size; p += channels) {
    int grayPexel;

    ++pexel;
    if (pexel % width == 0) {
      ++lines;
      if (lines % downscale_factor == 0) {
        fputc('\n', stdout);
      }
    }

    if (lines % downscale_factor || pexel % downscale_factor) {
      continue;
    }

    grayPexel = round((float)((p[0] + p[1] + p[2]) / 3));

    fputc(ASCII_CHARS[grayPexel / 25], stdout);
  }

  stbi_image_free(image);
  return 0;
}
