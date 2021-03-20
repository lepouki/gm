// Copyright (c) Amaël Marquez.  Licensed under the MIT License.
// See the LICENSE file at the root of the repository for all the details.

#include "resources.h"

#include "gm/error.h"
#include "model/model.h"
#include "program/program.h"
#include "setup.h"

gmError gmCreateRenderData_(GM_OUT_PARAM gmRenderData_ *render_data);

gmError gmCreateResources_(GM_OUT_PARAM gmResources_ *resources) {
  gmError error;

  error = gmCreateRenderData_(&resources->render_data);
  if (!error) {
  }

  return error;
}

gmError gmCreateRenderData_(GM_OUT_PARAM gmRenderData_ *render_data) {
  gmError error;

  error = gmCreateProgram_(&render_data->program);
  if (!error) {
    error = gmCreateQuadModel_(&render_data->quad);
    if (error) {
      gmDeleteProgram_(&render_data->program);
    }
  }

  return error;
}

void gmDeleteRenderData_(const gmRenderData_ *render_data);

void gmDeleteResources_(const gmResources_ *resources) {
  gmDeleteRenderData_(&resources->render_data);
}

void gmDeleteRenderData_(const gmRenderData_ *render_data) {
  gmDeleteModel_(&render_data->quad);
  gmDeleteProgram_(&render_data->program);
}
