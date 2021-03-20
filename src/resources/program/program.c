// Copyright (c) Amaël Marquez.  Licensed under the MIT License.
// See the LICENSE file at the root of the repository for all the details.

#include "program.h"

#include <glad/glad.h>

#include "check-status.h"
#include "gm/error.h"
#include "resources/gl-error.h"
#include "setup.h"
#include "shader.h"

gmError gmCheckProgramLinkStatus(const gmProgram_ *program);

typedef struct gmProgramShaders_ {
  gmShader_ vertex;
  gmShader_ fragment;
} gmProgramShaders_;

gmError gmCreateProgramShaders_(GM_OUT_PARAM gmProgramShaders_ *shaders);

gmError gmLinkProgram_(GM_OUT_PARAM gmProgram_ *program,
                       const gmProgramShaders_ *shaders);

void gmDeleteProgramShaders_(const gmProgramShaders_ *shaders);

gmError gmCreateProgram_(GM_OUT_PARAM gmProgram_ *program) {
  gmError error;

  gmProgramShaders_ shaders;
  error = gmCreateProgramShaders_(&shaders);
  if (!error) {
    error = gmLinkProgram_(program, &shaders);
    gmDeleteProgramShaders_(&shaders);  // Don't need the shaders anymore.
  }

  GM_GL_PRINT_ERROR_();

  return error;
}

// These files contain the shader sources.
#include "shaders/shaders.h"

gmError gmCreateProgramShaders_(GM_OUT_PARAM gmProgramShaders_ *shaders) {
  gmError error;

  error = gmCreateShader_(&shaders->vertex, gmShaderType_Vertex_,
                          kGmVertexShaderSource_);
  if (!error) {
    error = gmCreateShader_(&shaders->fragment, gmShaderType_Fragment_,
                            kGmFragmentShaderSource_);
    if (error) {
      gmDeleteShader_(&shaders->vertex);
    }
  }

  return error;
}

gmError gmLinkProgram_(GM_OUT_PARAM gmProgram_ *program,
                       const gmProgramShaders_ *shaders) {
  *program = glCreateProgram();

  glAttachShader(*program, shaders->vertex);
  glAttachShader(*program, shaders->fragment);

  glLinkProgram(*program);
  const gmError kError = gmCheckProgramLinkStatus(program);
  if (kError) {
    gmDeleteProgram_(program);
  }

  return kError;
}

void gmDeleteProgramShaders_(const gmProgramShaders_ *shaders) {
  gmDeleteShader_(&shaders->vertex);
  gmDeleteShader_(&shaders->fragment);
}

gmError gmCheckProgramLinkStatus(const gmProgram_ *program) {
  return gmCheckStatus_(*program, GL_LINK_STATUS, glGetProgramiv,
                        glGetProgramInfoLog);
}

void gmDeleteProgram_(const gmProgram_ *program) {
  glDeleteProgram(*program);
}

void gmUseProgram_(const gmProgram_ *program) {
  glUseProgram(*program);
}
