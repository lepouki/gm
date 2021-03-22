// Copyright (c) Amaël Marquez.  Licensed under the MIT License.
// See the LICENSE file at the root of the repository for all the details.

#include "frame-buffer.h"

#include <stdio.h>

#include "gm/error.h"
#include "gm/gm.h"
#include "resources/gl-error.h"
#include "setup.h"

/**
 * Function used to create the current render-buffer's storage.
 */
typedef void (*gmSetRenderBufferStorageFunc_)(
    const gmIntSize *size, GM_MAYBE_UNUSED gm_uint sample_count);

void gmCreateRenderBuffer_(
    GM_OUT_PARAM gmId_ *render_buffer,
    gmSetRenderBufferStorageFunc_ set_render_buffer_storage,
    const gmIntSize *size, gm_uint sample_count);

// The regular render-buffer storage does not need the sample info.
void gmSetRegularRenderBufferStorage_(const gmIntSize *size, gm_uint unused);

/**
 * Creates the frame-buffer and binds it to its color render-buffer.  This
 * function assumes the render-buffer has already been created.
 */
gmError gmCreateColorFrameBuffer_(GM_OUT_PARAM gmFrameBuffer_ *frame_buffer);

gmError gmCreateFrameBuffer_(GM_OUT_PARAM gmFrameBuffer_ *frame_buffer,
                             const gmIntSize *size) {
  gmCreateRenderBuffer_(&frame_buffer->color_render_buffer,
                        gmSetRegularRenderBufferStorage_, size, 0);

  // Deletes the render-buffer on failure.
  return gmCreateColorFrameBuffer_(frame_buffer);
}

void gmCreateRenderBuffer_(
    GM_OUT_PARAM gmId_ *render_buffer,
    gmSetRenderBufferStorageFunc_ set_render_buffer_storage,
    const gmIntSize *size, gm_uint sample_count) {
  glGenRenderbuffers(1, render_buffer);

  glBindRenderbuffer(GL_RENDERBUFFER, *render_buffer);
  set_render_buffer_storage(size, GM_MAYBE_UNUSED sample_count);
  glBindRenderbuffer(GL_RENDERBUFFER, 0);

  GM_GL_PRINT_ERROR_();
}

void gmSetRegularRenderBufferStorage_(const gmIntSize *size, gm_uint unused) {
  (void)unused;
  glRenderbufferStorage(GL_RENDERBUFFER, GL_RGB, size->w, size->h);
}

gmError gmCheckFrameBufferStatus_(const gmFrameBuffer_ *frame_buffer,
                                  gmFrameBufferTarget_ target);

gmError gmCreateColorFrameBuffer_(GM_OUT_PARAM gmFrameBuffer_ *frame_buffer) {
  glGenFramebuffers(1, &frame_buffer->id);

  const gmFrameBufferTarget_ kTarget = gmFrameBufferTarget_Framebuffer_;
  gmUseFrameBufferAs_(frame_buffer, kTarget);

  glFramebufferRenderbuffer(kTarget, GL_COLOR_ATTACHMENT0, GL_RENDERBUFFER,
                            frame_buffer->color_render_buffer);

  // Deletes the frame-buffer on failure.
  return gmCheckFrameBufferStatus_(frame_buffer, kTarget);
}

int gmIsFrameBufferComplete_(gmFrameBufferTarget_ target);

gmError gmCheckFrameBufferStatus_(const gmFrameBuffer_ *frame_buffer,
                                  gmFrameBufferTarget_ target) {
  const gmError kError = gmIsFrameBufferComplete_(target)
                             ? gmError_Success
                             : gmError_IncompleteFrameBuffer;
  if (kError) {
    gmDeleteFrameBuffer_(frame_buffer);
  }

  return kError;
}

int gmIsFrameBufferComplete_(gmFrameBufferTarget_ target) {
  return glCheckFramebufferStatus(target) == GL_FRAMEBUFFER_COMPLETE;
}

void gmSetSampledRenderBufferStorage_(const gmIntSize *size,
                                      gm_uint sample_count);

gmError gmCreateSampledFrameBuffer_(GM_OUT_PARAM gmFrameBuffer_ *frame_buffer,
                                    const gmIntSize *size,
                                    gm_uint sample_count) {
  gmCreateRenderBuffer_(&frame_buffer->color_render_buffer,
                        gmSetSampledRenderBufferStorage_, size, sample_count);

  // Deletes the render-buffer on failure.
  return gmCreateColorFrameBuffer_(frame_buffer);
}

gm_uint gmCheckSampleCountSupport_(gm_uint sample_count);

void gmSetSampledRenderBufferStorage_(const gmIntSize *size,
                                      gm_uint sample_count) {
  // Check if the specified sample count is supported by the GPU.
  sample_count = gmCheckSampleCountSupport_(sample_count);
  glRenderbufferStorageMultisample(GL_RENDERBUFFER, sample_count, GL_RGB,
                                   size->w, size->h);
}

gm_uint gmCheckSampleCountSupport_(gm_uint sample_count) {
  gm_uint final_sample_count = sample_count;

  int max_sample_count;
  glGetIntegerv(GL_MAX_SAMPLES, &max_sample_count);

  if (sample_count > max_sample_count) {
    final_sample_count = (gm_uint)max_sample_count;

    fprintf(stderr, "Unsupported sample count: reducing from %d to %d\n",
            sample_count, max_sample_count);
  }

  return final_sample_count;
}

void gmDeleteFrameBuffer_(const gmFrameBuffer_ *frame_buffer) {
  glDeleteFramebuffers(1, &frame_buffer->id);
  glDeleteRenderbuffers(1, &frame_buffer->color_render_buffer);
}

void gmClearCurrentFrameBuffer_(gmFrameBufferTarget_ target) {
  glBindFramebuffer(target, 0);
}

void gmUseFrameBufferAs_(const gmFrameBuffer_ *frame_buffer,
                         gmFrameBufferTarget_ target) {
  glBindFramebuffer(target, frame_buffer->id);
}
