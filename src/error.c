// Copyright (c) Amaël Marquez.  Licensed under the MIT License.
// See the LICENSE file at the root of the repository for all the details.

#include "gm/error.h"

const char *gmGetErrorMessage(gmError error) {
  switch (error) {
    case gmError_Success:
      return "Success";
    case gmError_GlfwInitFailed:
      return "Failed to initialize GLFW";
    case gmError_WindowCreationFailed:
      return "Failed to create the dummy window";
    case gmError_GlLoadingFailed:
      return "Failed to load OpenGL functions";
    case gmError_StatusCheckFailed:
      return "Resource status check failed";
    case gmError_IncompleteFrameBuffer:
      return "Failed to create a frame-buffer";
    case gmError_ImageWriteFailed:
      return "Failed to write the image";
    default:
      return "Unknown error";
  }
}
