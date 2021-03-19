// Copyright (c) Amaël Marquez.  Licensed under the MIT License.
// See the LICENSE file at the root of the repository for all the details.

#include "shader.h"

#include <glad/glad.h>

#include "check-status.h"

gmError gmCheckShaderCompileStatus_(const gmShader_ *shader);

gmError gmCreateShader_(GM_OUT_PARAM gmShader_ *shader, gmShaderType_ type,
                        const char *source) {
  return gmError_Success;
}

gmError gmCheckShaderCompileStatus_(const gmShader_ *shader) {
  return gmCheckStatus_(shader->id, GL_COMPILE_STATUS, glGetShaderiv,
                        glGetShaderInfoLog);
}

void gmDeleteShader_(const gmShader_ *shader) {
  glDeleteShader(shader->id);
}
