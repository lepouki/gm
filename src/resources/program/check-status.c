// Copyright (c) Amaël Marquez.  Licensed under the MIT License.
// See the LICENSE file at the root of the repository for all the details.

#include "check-status.h"

#include <glad/glad.h>

#include "gm/error.h"
#include "resources/id.h"
#include "setup.h"

#ifdef GM_DEBUG
void gmPrintInfo_(gmId_ object, gmGetivFunc_ getiv,
                  gmGetInfoLogFunc_ get_info_log);
#endif

gmError gmCheckStatus_(gmId_ object, GLenum status, gmGetivFunc_ getiv,
                       gmGetInfoLogFunc_ get_info_log) {
  int status_value;
  getiv(object, status, &status_value);

  if (!status_value) {
#ifdef GM_DEBUG
    gmPrintInfo_(object, getiv, get_info_log);
#endif
    return gmError_StatusCheckFailed;
  }

  return gmError_Success;
}

#ifdef GM_DEBUG
#  include <stdio.h>
#  include <stdlib.h>

void gmPrintInfo_(gmId_ object, gmGetivFunc_ getiv,
                  gmGetInfoLogFunc_ get_info_log) {
  int length;
  getiv(object, GL_INFO_LOG_LENGTH, &length);

  GLchar *info_log = malloc(length);
  get_info_log(object, length, NULL, info_log);

  fprintf(stderr, "Status check failed:\n%s", info_log);
  free(info_log);
}
#endif
