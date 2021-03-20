// Copyright (c) Amaël Marquez.  Licensed under the MIT License.
// See the LICENSE file at the root of the repository for all the details.

#define GLFW_INCLUDE_NONE

#include "context.h"

#include <GLFW/glfw3.h>
#include <glad/glad.h>

#include "gm/error.h"
#include "setup.h"

gmError gmInitGlfw_();
gmError gmLoadGl_(GM_OUT_PARAM gmContext_ *context);

void gmCleanupGlfw_();

gmError gmCreateContext_(GM_OUT_PARAM gmContext_ *context) {
  gmError error;

  // GLFW is initialized here because we're only using one context.
  error = gmInitGlfw_();
  if (!error) {
    error = gmLoadGl_(context);
    if (error) {
      gmCleanupGlfw_();
    }
  }

  return error;
}

gmError gmInitGlfw_() {
  return glfwInit() ? gmError_Success : gmError_GlfwInitFailed;
}

gmError gmCreateWindow_(GM_OUT_PARAM gmWindow_ *output_window);
gmError gmGladLoadGl_(const gmContext_ *context);

void gmDeleteWindow_(const gmWindow_ *window);

gmError gmLoadGl_(GM_OUT_PARAM gmContext_ *context) {
  gmError error;

  error = gmCreateWindow_(&context->window);
  if (!error) {
    error = gmGladLoadGl_(context);
    if (error) {
      gmDeleteWindow_(&context->window);
    }
  }

  return error;
}

void gmSetWindowHints_();

gmError gmCreateWindow_(GM_OUT_PARAM gmWindow_ *window) {
  gmSetWindowHints_();
  *window = glfwCreateWindow(1, 1, "", NULL, NULL);
  return *window ? gmError_Success : gmError_WindowCreationFailed;
}

void gmSetWindowHints_() {
  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
  glfwWindowHint(GLFW_VISIBLE, GLFW_FALSE);
}

#ifdef GM_DEBUG
void gmPrintGlInfo_();
#endif

gmError gmGladLoadGl_(const gmContext_ *context) {
  gmMakeContextCurrent_(context);

  const GLADloadproc kLoader = (GLADloadproc)glfwGetProcAddress;
  const gmError kError =
      gladLoadGLLoader(kLoader) ? gmError_Success : gmError_GlLoadingFailed;

#ifdef GM_DEBUG
  if (!kError) {
    gmPrintGlInfo_();
  }
#endif

  gmClearCurrentContext_();
  return kError;
}

#ifdef GM_DEBUG
#  include <stdio.h>
#  define GM_GET_GL_STRING_(name) (const char *)glGetString(name)

void gmPrintGlInfo_() {
  const char *const kVersion = GM_GET_GL_STRING_(GL_VERSION);
  const char *const kVendor = GM_GET_GL_STRING_(GL_VENDOR);
  const char *const kRenderer = GM_GET_GL_STRING_(GL_RENDERER);

  printf("OpenGL info:\n  Version: %s\n  Vendor: %s\n  Renderer: %s\n",
         kVersion, kRenderer, kVendor);
}
#endif

void gmDeleteWindow_(const gmWindow_ *window) {
  glfwDestroyWindow(*window);
}

void gmCleanupGlfw_() {
  glfwTerminate();
}

void gmDeleteContext_(const gmContext_ *context) {
  glfwDestroyWindow(context->window);

  // GLFW is cleaned up here because we're only using one context.
  gmCleanupGlfw_();
}

void gmClearCurrentContext_() {
  glfwMakeContextCurrent(NULL);
}

void gmMakeContextCurrent_(const gmContext_ *context) {
  glfwMakeContextCurrent(context->window);
}
