// Copyright (c) Amaël Marquez.  Licensed under the MIT License.
// See the LICENSE file at the root of the repository for all the details.

#include "program.h"

#include <glad/glad.h>

#include "check-status.h"

gmError gmCheckProgramLinkStatus(const gmProgram_ *program);

gmError gmCreateProgram_(GM_OUT_PARAM gmProgram_ *program) {
  return gmError_Success;
}

gmError gmCheckProgramLinkStatus(const gmProgram_ *program) {
  return gmCheckStatus_(program->id, GL_LINK_STATUS, glGetProgramiv,
                        glGetProgramInfoLog);
}

void gmDeleteProgram_(const gmProgram_ *program) {
  glDeleteProgram(program->id);
}

void gmUseProgram_(const gmProgram_ *program) {
  glUseProgram(program->id);
}
