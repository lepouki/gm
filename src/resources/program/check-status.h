// Copyright (c) Amaël Marquez.  Licensed under the MIT License.
// See the LICENSE file at the root of the repository for all the details.

#pragma once

#include <glad/glad.h>

#include "gm/error.h"
#include "resources/id.h"

typedef PFNGLGETSHADERIVPROC gmGetivFunc_;
typedef PFNGLGETSHADERINFOLOGPROC gmGetInfoLogFunc_;

gmError gmCheckStatus_(gmId_ object, GLenum status, gmGetivFunc_ getiv,
                       gmGetInfoLogFunc_ get_info_log);
