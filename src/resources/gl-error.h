// Copyright (c) Amaël Marquez.  Licensed under the MIT License.
// See the LICENSE file at the root of the repository for all the details.

#pragma once

#include <glad/glad.h>

#include "gm/setup.h"

/**
 * @return A human readable message corresponding to the specified OpenGL error
 * code.
 */
const char *gmGetGlErrorMessage_(GLenum error);

#ifdef GM_DEBUG
/**
 * Executes the specified statement and prints an error message if any error
 * occurred.
 */
#  define GM_GL_CALL_(statement)                                        \
    {                                                                   \
      statement;                                                        \
      const int kError = glGetError();                                  \
      if (kError) {                                                     \
        const char *const kErrorMessage = gmGetGlErrorMessage_(kError); \
        fprintf(stderr,                                                 \
                "OpenGL error: %s, file " __FILE__                      \
                ", line " GM_STRINGIFY(__LINE__) "\n",                  \
                kErrorMessage);                                         \
      }                                                                 \
    }
#else
#  define GM_GL_CALL_(...)
#endif
