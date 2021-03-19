// Copyright (c) Amaël Marquez.  Licensed under the MIT License.
// See the LICENSE file at the root of the repository for all the details.

#pragma once

#include <glad/glad.h>

#include "gm/error.h"
#include "resources/id.h"
#include "setup.h"

typedef enum gmShaderType_ {
  gmShaderType_Vertex_ = GL_VERTEX_SHADER,
  gmShaderType_Fragment_ = GL_FRAGMENT_SHADER
} gmShaderType_;

typedef gmId_ gmShader_;

gmError gmCreateShader_(GM_OUT_PARAM gmShader_ *shader, gmShaderType_ type,
                        const char *source);

void gmDeleteShader_(const gmShader_ *shader);
