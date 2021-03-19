// Copyright (c) Amaël Marquez.  Licensed under the MIT License.
// See the LICENSE file at the root of the repository for all the details.

#include "resources.h"

gmError gmCreateResources_(gmResources_ *resources) {
  gmError error;

  error = gmCreateProgram_(&resources->program);
  if (!error) {
  }

  return error;
}

void gmDeleteResources_(const gmResources_ *resources) {
  gmDeleteProgram_(&resources->program);
}
