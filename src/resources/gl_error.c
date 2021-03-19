// Copyright (c) Amaël Marquez.  Licensed under the MIT License.
// See the LICENSE file at the root of the repository for all the details.

#include "gl_error.h"

const char *gmGetGlErrorStr_(GLenum error) {
  switch (error) {
    case GL_NO_ERROR:
      return "(GL_NO_ERROR) No error has been recorded";
    case GL_INVALID_ENUM:
      return "(GL_INVALID_ENUM) Unacceptable value specified for an enum arg";
    case GL_INVALID_VALUE:
      return "(GL_INVALID_VALUE) Numeric arg out of range";
    case GL_INVALID_OPERATION:
      return "(GL_INVALID_OPERATION) Operation not allowed in current state";
    case GL_INVALID_FRAMEBUFFER_OPERATION:
      return "(GL_INVALID_FRAMEBUFFER_OPERATION): Framebuffer not complete";
    case GL_OUT_OF_MEMORY:
      return "(GL_OUT_OF_MEMORY): Not enough memory to execute";
    default:
      return "Unknown OpenGL error";
  }
}
