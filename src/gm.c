// Copyright (c) Amaël Marquez.  Licensed under the MIT License.
// See the LICENSE file at the root of the repository for all the details.

#include "gm/gm.h"

#include "context/context.h"
#include "resources/resources.h"

gmError gmRenderImage_(const gmConfig *config);

gmError gmRun(const gmConfig *config) {
  gmError error;

  gmContext_ context;
  error = gmCreateContext_(&context);
  if (!error) {
    error = gmRenderImage_(config);
    gmDeleteContext_(&context);
  }

  return error;
}

gmError gmRenderImage_(const gmConfig *config) {
  gmError error;

  gmResources_ resources;
  error = gmCreateResources_(&resources);
  if (!error) {
  }

  return error;
}
