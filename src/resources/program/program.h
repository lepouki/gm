// Copyright (c) Amaël Marquez.  Licensed under the MIT License.
// See the LICENSE file at the root of the repository for all the details.

#pragma once

#include "gm/error.h"
#include "resources/id.h"
#include "setup.h"

typedef gmId_ gmProgram_;

gmError gmCreateProgram_(GM_OUT_PARAM gmProgram_ *program);
void gmDeleteProgram_(const gmProgram_ *program);

void gmClearCurrentProgram_();
void gmUseProgram_(const gmProgram_ *program);
