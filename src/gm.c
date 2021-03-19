// Copyright (c) Amaël Marquez.  Licensed under the MIT License.
// See the LICENSE file at the root of the repository for all the details.

#include "gm/gm.h"

#include <glad/glad.h>
#include <stdio.h>  // Temporary.

#include "context/context.h"

/**
 * Assumes it is being called in a valid OpenGL context.
 */
void gmPrintGlInfo_();

gmError gmRun(const gmConfig *config) {
  gmError error;

  gmContext_ context;
  error = gmCreateContext_(&context);
  if (!error) {
    gmMakeContextCurrent_(&context);
    gmPrintGlInfo_();
    gmClearCurrentContext_();
    gmDeleteContext_(&context);
  }

  return error;
}

#define GM_GET_GL_STRING_(name) (const char *)glGetString(name)

void gmPrintGlInfo_() {
  const char *const kRenderer = GM_GET_GL_STRING_(GL_RENDERER);
  const char *const kVendor = GM_GET_GL_STRING_(GL_VENDOR);
  const char *const kVersion = GM_GET_GL_STRING_(GL_VERSION);

  printf("OpenGL info:\n  Renderer: %s\n  Vendor: %s\n  Version: %s\n",
         kRenderer, kVendor, kVersion);
}
