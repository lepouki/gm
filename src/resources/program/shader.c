// Copyright (c) Amaël Marquez.  Licensed under the MIT License.
// See the LICENSE file at the root of the repository for all the details.

#include "shader.h"

#include <glad/glad.h>
#include <stdlib.h>  // For NULL.

#include "check-status.h"
#include "gm/error.h"
#include "setup.h"

gmError gmCheckShaderCompileStatus_(const gmShader_ *shader);

gmError gmCreateShader_(GM_OUT_PARAM gmShader_ *shader, gmShaderType_ type,
                        const char *source) {
  *shader = glCreateShader(type);

  glShaderSource(*shader, 1, &source, NULL);
  glCompileShader(*shader);

  const gmError kError = gmCheckShaderCompileStatus_(shader);
  if (kError) {
    gmDeleteShader_(shader);
  }

  return kError;
}

gmError gmCheckShaderCompileStatus_(const gmShader_ *shader) {
  return gmCheckStatus_(*shader, GL_COMPILE_STATUS, glGetShaderiv,
                        glGetShaderInfoLog);
}

void gmDeleteShader_(const gmShader_ *shader) {
  glDeleteShader(*shader);
}
