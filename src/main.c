// Copyright (c) Amaël Marquez.  Licensed under the MIT License.
// See the LICENSE file at the root of the repository for all the details.

#include <stdio.h>

#include "gm/gm.h"

int main() {
  // Note that your GPU might not support as many as 32 samples, the sample
  // count will automatically be reduced to the max supported value.
  const gmConfig kConfig = {
      .image_config = {.size = {.w = 500, .h = 500}, .sample_count = 32},
      .image_output_filepath = "output.png"};

  const gmError kError = gmRun(&kConfig);
  if (kError) {
    const char *const kErrorMessage = gmGetErrorMessage(kError);
    fprintf(stderr, "Error: %s\n", kErrorMessage);
  }

  return kError;
}
