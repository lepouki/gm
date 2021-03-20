// Copyright (c) Amaël Marquez.  Licensed under the MIT License.
// See the LICENSE file at the root of the repository for all the details.

#pragma once

#include <glad/glad.h>

#include "gm/error.h"
#include "model/model.h"
#include "program/program.h"
#include "setup.h"

typedef struct gmRenderData_ {
  gmModel_ quad;
  gmProgram_ program;
} gmRenderData_;

typedef struct gmResources_ {
  gmRenderData_ render_data;
} gmResources_;

gmError gmCreateResources_(GM_OUT_PARAM gmResources_ *resources);
void gmDeleteResources_(const gmResources_ *resources);
