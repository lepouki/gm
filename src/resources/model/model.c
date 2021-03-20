// Copyright (c) Amaël Marquez.  Licensed under the MIT License.
// See the LICENSE file at the root of the repository for all the details.

#include "model.h"

#include <glad/glad.h>
#include <stdint.h>  // For intptr_t.

#include "buffer.h"
#include "gm/error.h"
#include "resources/gl-error.h"
#include "setup.h"

void gmLoadQuadData_(const gmModel_ *model);
void gmSpecifyModelBindings_(const gmModel_ *model);

gmError gmCreateQuadModel_(GM_OUT_PARAM gmModel_ *model) {
  gmError error;

  glGenVertexArrays(1, &model->vertex_array);

  error = gmCreateBuffers_(2, model->buffers);
  if (!error) {
    gmLoadQuadData_(model);
    gmSpecifyModelBindings_(model);
  } else {
    glDeleteVertexArrays(1, &model->vertex_array);
  }

  GM_GL_PRINT_ERROR_();

  return error;
}

// These files contain the quad data.
#include "quad/quad.h"

void gmLoadQuadData_(const gmModel_ *model) {
  gmUseBufferAs_(&model->buffers[0], gmBufferTarget_Vertex_);
  gmUseBufferAs_(&model->buffers[1], gmBufferTarget_Index_);

  gmLoadBufferDataAs_(gmBufferTarget_Vertex_, sizeof(kGmQuadVertices_),
                      kGmQuadVertices_);

  gmLoadBufferDataAs_(gmBufferTarget_Index_, sizeof(kGmQuadIndices_),
                      kGmQuadIndices_);

  gmClearCurrentBuffer_(gmBufferTarget_Vertex_);
  gmClearCurrentBuffer_(gmBufferTarget_Index_);
}

void gmSetQuadVertexAttributes_();

void gmSpecifyModelBindings_(const gmModel_ *model) {
  gmUseModel_(model);

  gmUseBufferAs_(&model->buffers[0], gmBufferTarget_Vertex_);
  gmUseBufferAs_(&model->buffers[1], gmBufferTarget_Index_);

  gmSetQuadVertexAttributes_();

  gmClearCurrentModel_();
  gmClearCurrentBuffer_(gmBufferTarget_Vertex_);
  gmClearCurrentBuffer_(gmBufferTarget_Index_);
}

typedef enum gmVertexComponentType_ {
  gmVertexComponentType_Float_ = GL_FLOAT
} gmVertexComponentType_;

typedef struct gmVertexAttribute_ {
  size_t count;
  gmVertexComponentType_ type;
} gmVertexAttribute_;

void gmSetVertexAttributes_(size_t count, const gmVertexAttribute_ *attributes);

void gmSetQuadVertexAttributes_() {
  // clang-format off
  const gmVertexAttribute_ kVertexAttributes[] = {
      // Position.
      {.count = 2, .type = gmVertexComponentType_Float_},

      // Texture coordinates.
      {.count = 2, .type = gmVertexComponentType_Float_}
  };
  // clang-format on

  const size_t kCount = sizeof(kVertexAttributes) / sizeof(gmVertexAttribute_);
  gmSetVertexAttributes_(kCount, kVertexAttributes);
}

size_t gmCalculateVertexStride_(size_t count,
                                const gmVertexAttribute_ *attributes);

size_t gmGetAttributeSize_(const gmVertexAttribute_ *attribute);

void gmSetVertexAttributes_(size_t count,
                            const gmVertexAttribute_ *attributes) {
  intptr_t offset = 0;
  const size_t kStride = gmCalculateVertexStride_(count, attributes);

  for (size_t i = 0; i < count; ++i) {
    const gmVertexAttribute_ kAttribute = attributes[i];

    glEnableVertexAttribArray(i);
    glVertexAttribPointer(i, kAttribute.count, kAttribute.type, GL_FALSE,
                          kStride, (const void *)offset);

    offset += gmGetAttributeSize_(&kAttribute);
  }
}

size_t gmGetComponentSize_(gmVertexComponentType_ type);

size_t gmGetAttributeSize_(const gmVertexAttribute_ *attribute) {
  return gmGetComponentSize_(attribute->type) * attribute->count;
}

size_t gmGetComponentSize_(gmVertexComponentType_ type) {
  // NOLINTNEXTLINE
  switch (type) {
    case gmVertexComponentType_Float_:
      return sizeof(float);
    default:
      return 0;  // This should never happen.
  }
}

size_t gmCalculateVertexStride_(size_t count,
                                const gmVertexAttribute_ *attributes) {
  size_t stride = 0;

  for (size_t i = 0; i < count; ++i) {
    const gmVertexAttribute_ kAttribute = attributes[i];
    stride += gmGetAttributeSize_(&kAttribute);
  }

  return stride;
}

void gmDeleteModel_(const gmModel_ *model) {
  gmDeleteBuffers_(2, model->buffers);
  glDeleteVertexArrays(1, &model->vertex_array);
}

void gmClearCurrentModel_() {
  glBindVertexArray(0);
}

void gmUseModel_(const gmModel_ *model) {
  glBindVertexArray(model->vertex_array);
}
