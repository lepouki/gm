// Copyright (c) Amaël Marquez.  Licensed under the MIT License.
// See the LICENSE file at the root of the repository for all the details.

#define STB_IMAGE_WRITE_IMPLEMENTATION

#include "gm/gm.h"

#include <stb/stb_image_write.h>

#include "context/context.h"
#include "gm/error.h"
#include "resources/resources.h"

gmError gmRenderImageToFile_(const gmConfig *config);

gmError gmRun(const gmConfig *config) {
  gmError error;

  gmContext_ context;
  error = gmCreateContext_(&context);
  if (!error) {
    gmMakeContextCurrent_(&context);
    error = gmRenderImageToFile_(config);

    gmClearCurrentContext_();
    gmDeleteContext_(&context);
  }

  return error;
}

void gmRenderImage_(const gmResources_ *resources,
                    const gmImageConfig *image_config);

gmError gmSaveImage_(const gmFrameBuffer_ *final_frame_buffer,
                     const gmConfig *config);

gmError gmRenderImageToFile_(const gmConfig *config) {
  gmError error;

  gmResources_ resources;
  error = gmCreateResources_(&resources, &config->image_config);
  if (!error) {
    gmRenderImage_(&resources, &config->image_config);
    error = gmSaveImage_(&resources.render_frame_buffers.final, config);
    gmDeleteResources_(&resources);
  }

  return error;
}

void gmRenderImageOnRenderFrameBuffer_(const gmResources_ *resources);

void gmBlitToFinalFrameBuffer_(const gmRenderFrameBuffers_ *frame_buffers,
                               const gmIntSize *image_size);

void gmRenderImage_(const gmResources_ *resources,
                    const gmImageConfig *image_config) {
  // Not setting the viewport results in the image not rendering entirely.
  glViewport(0, 0, image_config->size.w, image_config->size.h);
  gmRenderImageOnRenderFrameBuffer_(resources);

  gmBlitToFinalFrameBuffer_(&resources->render_frame_buffers,
                            &image_config->size);
}

void gmRenderImageOnRenderFrameBuffer_(const gmResources_ *resources) {
  gmUseFrameBufferAs_(&resources->render_frame_buffers.render,
                      gmFramebufferTarget_Draw_);

  gmUseModel_(&resources->render_data.quad);
  gmUseProgram_(&resources->render_data.program);

  // Hard coded because we're only rendering one quad.
  glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_SHORT, NULL);

  gmClearCurrentFrameBuffer_(gmFramebufferTarget_Draw_);
  gmClearCurrentModel_();
  gmClearCurrentProgram_();
}

void gmBlitToFinalFrameBuffer_(const gmRenderFrameBuffers_ *frame_buffers,
                               const gmIntSize *image_size) {
  gmUseFrameBufferAs_(&frame_buffers->final, gmFramebufferTarget_Draw_);
  gmUseFrameBufferAs_(&frame_buffers->render, gmFramebufferTarget_Read_);

  glBlitFramebuffer(0, 0, image_size->w, image_size->h, 0, 0, image_size->w,
                    image_size->h, GL_COLOR_BUFFER_BIT, GL_LINEAR);

  gmClearCurrentFrameBuffer_(gmFramebufferTarget_Read_);
  gmClearCurrentFrameBuffer_(gmFramebufferTarget_Draw_);
}

void gmReadImageData_(GM_OUT_PARAM void *image_data,
                      const gmFrameBuffer_ *final_frame_buffer,
                      const gmIntSize *image_size);

gmError gmWriteImageToFile_(unsigned char *image_data, const gmConfig *config);

gmError gmSaveImage_(const gmFrameBuffer_ *final_frame_buffer,
                     const gmConfig *config) {
  const gmIntSize *kSize = &config->image_config.size;
  unsigned char *const kImageData = malloc(kSize->w * kSize->h * 3);  // RGB.

  gmReadImageData_(kImageData, final_frame_buffer, &config->image_config.size);
  const gmError kError = gmWriteImageToFile_(kImageData, config);

  free(kImageData);
  return kError;
}

void gmReadImageData_(GM_OUT_PARAM void *image_data,
                      const gmFrameBuffer_ *final_frame_buffer,
                      const gmIntSize *image_size) {
  glReadBuffer(GL_COLOR_ATTACHMENT0);
  gmUseFrameBufferAs_(final_frame_buffer, gmFramebufferTarget_Read_);

  glReadPixels(0, 0, image_size->w, image_size->h, GL_RGB, GL_UNSIGNED_BYTE,
               image_data);
}

gmError gmWriteImageToFile_(unsigned char *image_data, const gmConfig *config) {
  const gmIntSize *kImageSize = &config->image_config.size;
  const int kLineStride = kImageSize->w * 3;

  const int kError =
      !stbi_write_png(config->image_output_filepath, kImageSize->w,
                      kImageSize->h, 3, image_data, kLineStride);

  return !kError ? gmError_Success : gmError_ImageWriteFailed;
}
