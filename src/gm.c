// Copyright (c) Amaël Marquez.  Licensed under the MIT License.
// See the LICENSE file at the root of the repository for all the details.

#include "gm/gm.h"

#include "context/context.h"

gmError gmRun(const gmConfig *config) {
  gmError error;

  gmContext_ context;
  error = gmNewContext_(&context);

  if (!error) {
    // Render the image.

    gmDeleteContext_(&context);
    return gmError_Success;
  } else {
    return error;
  }
}
