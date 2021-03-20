// Copyright (c) Amaël Marquez.  Licensed under the MIT License.
// See the LICENSE file at the root of the repository for all the details.

#pragma once

#include "error.h"

typedef struct gmIntSize {
  int w;
  int h;
} gmIntSize;

typedef struct gmImageConfig {
  gmIntSize size;
  unsigned int samples;
} gmImageConfig;

typedef struct gmConfig {
  const char *image_output_filepath;
  gmImageConfig image_config;
} gmConfig;

/**
 * Renders the Mandelbrot set image using the specified config.
 */
gmError gmRun(const gmConfig *config);
