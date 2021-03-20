// Copyright (c) Amaël Marquez.  Licensed under the MIT License.
// See the LICENSE file at the root of the repository for all the details.

#include <stdio.h>

#include "gm/gm.h"

int main() {
  gmConfig config = {
      .image_config = {.size = {.w = 500, .h = 500}, .samples = 32},
      .image_output_filepath = "output.png"};

  const gmError kError = gmRun(&config);
  if (kError) {
    const char *const kErrorMessage = gmGetErrorMessage(kError);
    fprintf(stderr, "Error: %s\n", kErrorMessage);
  }

  return kError;
}
