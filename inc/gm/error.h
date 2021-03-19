// Copyright (c) Amaël Marquez.  Licensed under the MIT License.
// See the LICENSE file at the root of the repository for all the details.

#pragma once

typedef enum gmError {
  gmError_Success,
  gmError_GlfwInitFailed,
  gmError_WindowCreationFailed,
  gmError_GlLoadingFailed
} gmError;

/**
 * @return A human readable message corresponding to the specified error code.
 */
const char *gmGetErrorMessage(gmError error);
