// Copyright (c) Amaël Marquez.  Licensed under the MIT License.
// See the LICENSE file at the root of the repository for all the details.

#include "gm/gm.h"

#include <glad/glad.h>
#include <stdio.h>  // Temporary.

#include "context/context.h"

gmError gmRun(const gmConfig *config) {
  gmError error;

  gmContext_ context;
  error = gmCreateContext_(&context);
  if (!error) {
    gmMakeContextCurrent_(&context);

    const char *const kRenderer = (const char *)glGetString(GL_RENDERER);
    const char *const kVendor = (const char *)glGetString(GL_VENDOR);
    const char *const kVersion = (const char *)glGetString(GL_VERSION);

    printf("OpenGL info:\n- Renderer: %s\n- Vendor: %s\n- Version: %s\n",
           kRenderer, kVendor, kVersion);

    gmClearCurrentContext_();
    gmDeleteContext_(&context);
  }

  return error;
}
