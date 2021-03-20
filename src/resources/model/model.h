// Copyright (c) Amaël Marquez.  Licensed under the MIT License.
// See the LICENSE file at the root of the repository for all the details.

#pragma once

#include "buffer.h"
#include "gm/error.h"
#include "resources/id.h"
#include "setup.h"

typedef struct gmModel_ {
  gmId_ vertex_array;
  gmBuffer_ buffers[2];
} gmModel_;

gmError gmCreateQuadModel_(GM_OUT_PARAM gmModel_ *model);
void gmDeleteModel_(const gmModel_ *model);

void gmClearCurrentModel_();
void gmUseModel_(const gmModel_ *model);
