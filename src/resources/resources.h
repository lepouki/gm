// Copyright (c) Amaël Marquez.  Licensed under the MIT License.
// See the LICENSE file at the root of the repository for all the details.

#pragma once

#include <glad/glad.h>

#include "gm/error.h"
#include "gm/setup.h"
#include "id.h"
#include "program/program.h"

typedef struct gmResources_ {
  gmProgram_ program;
} gmResources_;

gmError gmCreateResources_(GM_OUT_PARAM gmResources_ *resources);
void gmDeleteResources_(const gmResources_ *resources);
