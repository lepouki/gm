// Copyright (c) Amaël Marquez.  Licensed under the MIT License.
// See the LICENSE file at the root of the repository for all the details.

#pragma once

#include "gm/error.h"

typedef struct GLFWwindow GLFWwindow;

typedef struct gmContext_ {
  GLFWwindow *window;
} gmContext_;

gmError gmNewContext_(gmContext_ *out_context);
void gmDeleteContext_(const gmContext_ *context);
