// Copyright (c) Amaël Marquez.  Licensed under the MIT License.
// See the LICENSE file at the root of the repository for all the details.

#include "frame-buffer.h"

#include "gm/error.h"
#include "gm/gm.h"
#include "resources/gl-error.h"
#include "setup.h"

/**
 * Function used to create the current render-buffer's storage.
 */
typedef void (*gmSetRenderBufferStorageFunc_)(
    const gmIntSize *size, GM_MAYBE_UNUSED unsigned int samples);

void gmCreateRenderBuffer_(
    GM_OUT_PARAM gmId_ *render_buffer,
    gmSetRenderBufferStorageFunc_ set_render_buffer_storage,
    const gmIntSize *size, unsigned int samples);

// The regular render-buffer storage does not need the sample info.
void gmSetRegularRenderBufferStorage_(const gmIntSize *size,
                                      unsigned int unused);

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
    const gmIntSize *size, unsigned int samples) {
  glGenRenderbuffers(1, render_buffer);

  glBindRenderbuffer(GL_RENDERBUFFER, *render_buffer);
  set_render_buffer_storage(size, GM_MAYBE_UNUSED samples);
  glBindRenderbuffer(GL_RENDERBUFFER, 0);

  GM_GL_PRINT_ERROR_();
}

void gmSetRegularRenderBufferStorage_(const gmIntSize *size,
                                      unsigned int unused) {
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
                                      unsigned int samples);

gmError gmCreateSampledFrameBuffer_(GM_OUT_PARAM gmFrameBuffer_ *frame_buffer,
                                    const gmIntSize *size,
                                    unsigned int samples) {
  gmCreateRenderBuffer_(&frame_buffer->color_render_buffer,
                        gmSetSampledRenderBufferStorage_, size, samples);

  // Deletes the render-buffer on failure.
  return gmCreateColorFrameBuffer_(frame_buffer);
}

void gmSetSampledRenderBufferStorage_(const gmIntSize *size,
                                      unsigned int samples) {
  // No idea how to verify the max sample count, so just rely on the
  // completeness check.
  glRenderbufferStorageMultisample(GL_RENDERBUFFER, samples, GL_RGB, size->w,
                                   size->h);
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
