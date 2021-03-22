// Copyright (c) Amaël Marquez.  Licensed under the MIT License.
// See the LICENSE file at the root of the repository for all the details.

#pragma once

#include <glad/glad.h>

#include "setup.h"

/**
 * @return A human readable message corresponding to the specified OpenGL error
 * code.
 */
const char *gmGetGlErrorMessage_(GLenum error);

#define GM_FORCE_SEMICOLON_ (void)0

#ifdef GM_DEBUG
#  include <stdio.h>

// clang-format off
#  define GM_STRINGIFY_HELPER_(x) #x
#  define GM_STRINGIFY_(x) GM_STRINGIFY_HELPER_(x)
// clang-format on

/**
 * Executes the specified statement and prints an error message if any error
 * occurred.
 */
#  define GM_GL_PRINT_ERROR_()                                            \
    {                                                                     \
      const int kGlError = glGetError();                                  \
      if (kGlError) {                                                     \
        const char *const kErrorMessage = gmGetGlErrorMessage_(kGlError); \
        fprintf(stderr,                                                   \
                "OpenGL error: %s (" __FILE__                             \
                ":" GM_STRINGIFY_(__LINE__) ")\n",                        \
                kErrorMessage);                                           \
      }                                                                   \
    }                                                                     \
    GM_FORCE_SEMICOLON_
#else
#  define GM_GL_PRINT_ERROR_() GM_FORCE_SEMICOLON_
#endif
