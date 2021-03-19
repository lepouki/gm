// Copyright (c) Amaël Marquez.  Licensed under the MIT License.
// See the LICENSE file at the root of the repository for all the details.

#include "gm/error.h"

const char *gmGetErrorMessage(gmError status) {
  switch (status) {
    case gmError_Success:
      return "Success";
    case gmError_GlfwInitFailed:
      return "Failed to initialize GLFW";
    case gmError_WindowCreationFailed:
      return "Failed to create the dummy window";
    default:
      return "Unknown error";
  }
}
