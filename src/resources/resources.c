// Copyright (c) Amaël Marquez.  Licensed under the MIT License.
// See the LICENSE file at the root of the repository for all the details.

#include "resources.h"

#include "frame-buffer/frame-buffer.h"
#include "gm/error.h"
#include "model/model.h"
#include "program/program.h"
#include "setup.h"

gmError gmCreateRenderData_(GM_OUT_PARAM gmRenderData_ *render_data);

gmError gmCreateRenderFrameBuffers_(
    GM_OUT_PARAM gmRenderFrameBuffers_ *render_frame_buffers,
    const gmImageConfig *image_config);

void gmDeleteRenderData_(const gmRenderData_ *render_data);

gmError gmCreateResources_(GM_OUT_PARAM gmResources_ *resources,
                           const gmImageConfig *image_config) {
  gmError error;

  error = gmCreateRenderData_(&resources->render_data);
  if (!error) {
    error = gmCreateRenderFrameBuffers_(&resources->render_frame_buffers,
                                        image_config);
    if (error) {
      gmDeleteRenderData_(&resources->render_data);
    }
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

gmError gmCreateRenderFrameBuffers_(
    GM_OUT_PARAM gmRenderFrameBuffers_ *render_frame_buffers,
    const gmImageConfig *image_config) {
  gmError error;

  error = gmCreateSampledFrameBuffer_(&render_frame_buffers->render,
                                      &image_config->size,
                                      image_config->sample_count);
  if (!error) {
    error =
        gmCreateFrameBuffer_(&render_frame_buffers->final, &image_config->size);
    if (error) {
      gmDeleteFrameBuffer_(&render_frame_buffers->render);
    }
  }

  return error;
}

void gmDeleteRenderData_(const gmRenderData_ *render_data) {
  gmDeleteModel_(&render_data->quad);
  gmDeleteProgram_(&render_data->program);
}

void gmDeleteRenderFrameBuffers_(
    const gmRenderFrameBuffers_ *render_frame_buffers);

void gmDeleteResources_(const gmResources_ *resources) {
  gmDeleteRenderData_(&resources->render_data);
  gmDeleteRenderFrameBuffers_(&resources->render_frame_buffers);
}

void gmDeleteRenderFrameBuffers_(
    const gmRenderFrameBuffers_ *render_frame_buffers) {
  gmDeleteFrameBuffer_(&render_frame_buffers->final);
  gmDeleteFrameBuffer_(&render_frame_buffers->render);
}
