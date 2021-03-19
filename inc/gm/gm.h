// Copyright (c) Amaël Marquez.  Licensed under the MIT License.
// See the LICENSE file at the root of the repository for all the details.

#pragma once

#include "error.h"

typedef struct gmConfig {
  const char *image_output_filepath;
} gmConfig;

/**
 * Renders the Mandelbrot set image using the specified config.
 */
gmError gmRun(const gmConfig *config);
