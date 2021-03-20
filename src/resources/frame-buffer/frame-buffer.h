// Copyright (c) Amaël Marquez.  Licensed under the MIT License.
// See the LICENSE file at the root of the repository for all the details.

#pragma once

#include <glad/glad.h>

#include "gm/error.h"
#include "gm/gm.h"
#include "resources/id.h"
#include "setup.h"

typedef struct gmFrameBuffer_ {
  gmId_ id;
  gmId_ color_render_buffer;
} gmFrameBuffer_;

gmError gmCreateFrameBuffer_(GM_OUT_PARAM gmFrameBuffer_ *frame_buffer,
                             const gmIntSize *size);

gmError gmCreateSampledFrameBuffer_(GM_OUT_PARAM gmFrameBuffer_ *frame_buffer,
                                    const gmIntSize *size,
                                    unsigned int samples);

void gmDeleteFrameBuffer_(const gmFrameBuffer_ *frame_buffer);

typedef enum gmFrameBufferTarget_ {
  gmFramebufferTarget_Read_ = GL_READ_FRAMEBUFFER,
  gmFramebufferTarget_Draw_ = GL_DRAW_FRAMEBUFFER,

  /**
   * Acts as both read and draw frame-buffers.
   */
  gmFrameBufferTarget_Framebuffer_ = GL_FRAMEBUFFER
} gmFrameBufferTarget_;

void gmClearCurrentFrameBuffer_(gmFrameBufferTarget_ target);

void gmUseFrameBufferAs_(const gmFrameBuffer_ *frame_buffer,
                         gmFrameBufferTarget_ target);
