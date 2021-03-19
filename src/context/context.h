// Copyright (c) Amaël Marquez.  Licensed under the MIT License.
// See the LICENSE file at the root of the repository for all the details.

#pragma once

#include "gm/error.h"
#include "gm/setup.h"

typedef struct GLFWwindow *gmWindow_;

typedef struct gmContext_ {
  gmWindow_ window;
} gmContext_;

gmError gmCreateContext_(GM_OUT_PARAM gmContext_ *context);
void gmDeleteContext_(const gmContext_ *context);

void gmClearCurrentContext_();
void gmMakeContextCurrent_(const gmContext_ *context);
