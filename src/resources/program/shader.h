// Copyright (c) Amaël Marquez.  Licensed under the MIT License.
// See the LICENSE file at the root of the repository for all the details.

#pragma once

#include <glad/glad.h>

#include "gm/error.h"
#include "gm/setup.h"
#include "resources/id.h"

typedef enum gmShaderType_ {
  gmShaderType_Vertex,
  gmShaderType_Fragment
} gmShaderType_;

typedef struct gmShader_ {
  gmId_ id;
} gmShader_;

gmError gmCreateShader_(GM_OUT_PARAM gmShader_ *shader, gmShaderType_ type,
                        const char *source);

void gmDeleteShader_(const gmShader_ *shader);
