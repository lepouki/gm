// Copyright (c) Amaël Marquez.  Licensed under the MIT License.
// See the LICENSE file at the root of the repository for all the details.

#pragma once

#include "gm/error.h"
#include "gm/setup.h"
#include "resources/id.h"

typedef struct gmProgram_ {
  gmId_ id;
} gmProgram_;

gmError gmCreateProgram_(GM_OUT_PARAM gmProgram_ *program);
void gmDeleteProgram_(const gmProgram_ *program);

void gmUseProgram_(const gmProgram_ *program);
