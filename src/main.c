#define GLFW_INCLUDE_NONE
#define STB_IMAGE_WRITE_IMPLEMENTATION

#include <GLFW/glfw3.h>
#include <glad/glad.h>
#include <stb/stb_image_write.h>
#include <stdio.h>
#include <stdlib.h>

typedef enum {
  gm_Status_Success,
  gm_Status_GlfwInitFailed,
  gm_Status_DummyWindowCreationFailed,
  gm_Status_GlLoadingFailed,
  gm_Status_ShaderCompilationFailed,
  gm_Status_ProgramLinkingFailed,
  gm_Status_FramebufferCreationFailed,
  gm_Status_ImageOutputFailed
} gm_Status;

typedef struct {
  int x;
  int y;
} gm_IntSize;

typedef struct {
  gm_IntSize size;
  int samples;
} gm_ImageConfig;

gm_Status gm_Run(const gm_ImageConfig *config);
void gm_PrintStatus(FILE *stream, int status_code);

int main() {
  const gm_ImageConfig kConfig = {.size = {.x = 500, .y = 500}, .samples = 8};
  const int kStatus = gm_Run(&kConfig);

  if (kStatus) {
    gm_PrintStatus(stderr, kStatus);
  } else {
    gm_PrintStatus(stdout, kStatus);
  }

  return kStatus;
}

#define GM_FORCE_TRAILING_SEMICOLON_ (void)0

#define GM_PASS_OR_RETURN_STATUS(statement) \
  {                                         \
    const gm_Status kStatus = statement;    \
    if (kStatus) {                          \
      return kStatus;                       \
    }                                       \
  }                                         \
  GM_FORCE_TRAILING_SEMICOLON_

gm_Status gm_Init();

typedef GLuint gm_Id;

typedef struct {
  gm_Id program;
} gm_RenderResources;

gm_Status gm_CreateProgram(gm_RenderResources *out_resources);

#ifndef NDEBUG
#  define GM_PRINT_GL_ERROR_STATUS(info) \
    fprintf(stdout, "Error status: %d (%s)\n", glGetError(), info)
#else
#  define GM_PRINT_GL_ERROR_STATUS(...)
#endif

void gm_Terminate();

gm_Status gm_Run(const gm_ImageConfig *config) {
  GM_PASS_OR_RETURN_STATUS(gm_Init());

  gm_RenderResources resources;
  GM_PASS_OR_RETURN_STATUS(gm_CreateProgram(&resources));

  // Create the quad data.
  // clang-format off
  const GLfloat kVertices[] = { -1.0f, -1.0f, 0.0f, 0.0f,
                                -1.0f,  1.0f, 0.0f, 1.0f,
                                 1.0f,  1.0f, 1.0f, 1.0f,
                                 1.0f, -1.0f, 1.0f, 0.0f };

  const GLshort kIndices[] = { 0, 1, 3,
                               1, 2, 3 };
  // clang-format on

  // Create the quad vertex array.
  GLuint quad_va;
  glGenVertexArrays(1, &quad_va);
  glBindVertexArray(quad_va);

  // Create the quad buffers.
  GLuint buffers[2];
  glGenBuffers(2, buffers);
  GM_PRINT_GL_ERROR_STATUS("create quad buffers");

  // Send the data to the buffers.
  glBindBuffer(GL_ARRAY_BUFFER, buffers[0]);
  glBufferData(GL_ARRAY_BUFFER, sizeof(kVertices), kVertices, GL_STATIC_DRAW);
  GM_PRINT_GL_ERROR_STATUS("upload vertex data");

  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, buffers[1]);
  glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(kIndices), kIndices,
               GL_STATIC_DRAW);
  GM_PRINT_GL_ERROR_STATUS("upload index data");

  // Vertex layout.
  const GLsizei kStride = 4 * sizeof(GLfloat);

  glEnableVertexAttribArray(0);
  glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, kStride, NULL);

  const void *kTexCoordsOffset = (void *)(2 * sizeof(GLfloat));
  glEnableVertexAttribArray(1);
  glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, kStride, kTexCoordsOffset);
  GM_PRINT_GL_ERROR_STATUS("set vertex attribs");

  glBindVertexArray(0);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
  glBindBuffer(GL_ARRAY_BUFFER, 0);

  // Create and initialize the framebuffer renderbuffer.
  GLuint image_render_buffer;
  glGenRenderbuffers(1, &image_render_buffer);
  glBindRenderbuffer(GL_RENDERBUFFER, image_render_buffer);
  GM_PRINT_GL_ERROR_STATUS("create image render buffer");

  glRenderbufferStorageMultisample(GL_RENDERBUFFER, config->samples, GL_RGB8,
                                   config->size.x, config->size.y);
  GM_PRINT_GL_ERROR_STATUS("init image render buffer");

  glBindRenderbuffer(GL_RENDERBUFFER, 0);

  // Create the framebuffer and use it.
  GLuint render_framebuffer;
  glGenFramebuffers(1, &render_framebuffer);
  glBindFramebuffer(GL_FRAMEBUFFER, render_framebuffer);
  GM_PRINT_GL_ERROR_STATUS("create render framebuffer");

  // Attach it to the framebuffer.
  glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0,
                            GL_RENDERBUFFER, image_render_buffer);
  GM_PRINT_GL_ERROR_STATUS("attach image render buffer to render framebuffer");

  if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE) {
    return gm_Status_FramebufferCreationFailed;
  }

  glBindFramebuffer(GL_FRAMEBUFFER, 0);

  // Not doing this results in the image missing some parts.
  glViewport(0, 0, config->size.x, config->size.y);
  GM_PRINT_GL_ERROR_STATUS("set viewport");

  // Render a quad on the framebuffer.
  glBindFramebuffer(GL_FRAMEBUFFER, render_framebuffer);
  glUseProgram(resources.program);
  glBindVertexArray(quad_va);

  glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_SHORT, NULL);
  glBindFramebuffer(GL_FRAMEBUFFER, 0);
  GM_PRINT_GL_ERROR_STATUS("render the image");

  // Create the final image buffer.
  GLuint final_buffer;
  glGenRenderbuffers(1, &final_buffer);
  glBindRenderbuffer(GL_RENDERBUFFER, final_buffer);
  GM_PRINT_GL_ERROR_STATUS("create final buffer");

  glRenderbufferStorage(GL_RENDERBUFFER, GL_RGB, config->size.x,
                        config->size.y);
  GM_PRINT_GL_ERROR_STATUS("init final buffer");

  glBindRenderbuffer(GL_RENDERBUFFER, 0);

  // Create the final image framebuffer.
  GLuint final_framebuffer;
  glGenFramebuffers(1, &final_framebuffer);
  glBindFramebuffer(GL_FRAMEBUFFER, final_framebuffer);
  GM_PRINT_GL_ERROR_STATUS("create final framebuffer");

  glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0,
                            GL_RENDERBUFFER, final_buffer);
  GM_PRINT_GL_ERROR_STATUS("attach final buffer to final framebuffer");

  // Blit the render data to the final framebuffer.
  glBindFramebuffer(GL_READ_FRAMEBUFFER, render_framebuffer);
  glBindFramebuffer(GL_DRAW_FRAMEBUFFER, final_framebuffer);
  glBlitFramebuffer(0, 0, config->size.x, config->size.y, 0, 0, config->size.x,
                    config->size.y, GL_COLOR_BUFFER_BIT, GL_LINEAR);
  GM_PRINT_GL_ERROR_STATUS("blit image to final framebuffer");

  glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0);

  // Retrieve the image data from the framebuffer.
  unsigned char *data = malloc(config->size.x * config->size.y * 3);  // RGB.

  glBindFramebuffer(GL_READ_FRAMEBUFFER, final_framebuffer);
  GM_PRINT_GL_ERROR_STATUS("bind final framebuffer to read framebuffer");

  glReadBuffer(GL_COLOR_ATTACHMENT0);
  glReadPixels(0, 0, config->size.x, config->size.y, GL_RGB, GL_UNSIGNED_BYTE,
               data);
  GM_PRINT_GL_ERROR_STATUS("read image pixels");

  // Write the image data to a file.
  const int kLineStride = 3 * config->size.x;
  const int kResult = stbi_write_png("output.png", config->size.x,
                                     config->size.y, 3, data, kLineStride);

  if (!kResult) {
    fputs("Failed to write image\n", stderr);
    return gm_Status_ImageOutputFailed;
  }

  // Cleanup data.
  glDeleteRenderbuffers(1, &final_buffer);
  glDeleteFramebuffers(1, &final_framebuffer);
  glDeleteRenderbuffers(1, &image_render_buffer);
  glDeleteFramebuffers(1, &render_framebuffer);
  glDeleteProgram(resources.program);
  glDeleteBuffers(2, buffers);
  GM_PRINT_GL_ERROR_STATUS("delete objects");

  gm_Terminate();
  return gm_Status_Success;
}

/**
 * Dummy window used to create an OpenGL context.
 */
GLFWwindow *window;

gm_Status gm_Init() {
  if (!glfwInit()) {
    return gm_Status_GlfwInitFailed;
  }

  glfwDefaultWindowHints();
  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
  glfwWindowHint(GLFW_VISIBLE, GLFW_FALSE);

  // Dummy window.
  window = glfwCreateWindow(1, 1, "", NULL, NULL);
  if (!window) {
    return gm_Status_DummyWindowCreationFailed;
  }

  glfwMakeContextCurrent(window);
  if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
    return gm_Status_GlLoadingFailed;
  }

  return gm_Status_Success;
}

typedef struct {
  gm_Id vertex;
  gm_Id fragment;
} gm_ProgramShaders;

gm_Status gm_CreateShaders(gm_ProgramShaders *out_shaders);
void gm_DestroyShaders(const gm_ProgramShaders *shaders);
gm_Status gm_CheckProgramLinkStatus(GLuint program);

gm_Status gm_CreateProgram(gm_RenderResources *out_resources) {
  gm_ProgramShaders shaders;
  GM_PASS_OR_RETURN_STATUS(gm_CreateShaders(&shaders));

  out_resources->program = glCreateProgram();

  glAttachShader(out_resources->program, shaders.vertex);
  glAttachShader(out_resources->program, shaders.fragment);
  GM_PRINT_GL_ERROR_STATUS("attach shaders to program");

  glLinkProgram(out_resources->program);
  GM_PRINT_GL_ERROR_STATUS("link program");

  // We don't need the shaders anymore.
  gm_DestroyShaders(&shaders);
  GM_PASS_OR_RETURN_STATUS(gm_CheckProgramLinkStatus(out_resources->program));

  return gm_Status_Success;
}

gm_Status gm_CheckProgramLinkStatus(GLuint program) {
  int link_status;
  glGetProgramiv(program, GL_LINK_STATUS, &link_status);

  if (!link_status) {
    int length;
    glGetProgramiv(program, GL_INFO_LOG_LENGTH, &length);

    GLchar *info_log = malloc(length);
    glGetProgramInfoLog(program, length, NULL, info_log);

    fprintf(stderr, "Program linking failed:\n%s", info_log);
    return gm_Status_ProgramLinkingFailed;
  }

  return gm_Status_Success;
}

// clang-format off
const char *const kGm_VertexShaderSource =
    "#version 330 core \n"

    "layout(location = 0) in vec2 a_Position;\n"
    "layout(location = 1) in vec2 a_TexCoords;\n"

    "out vec2 v_TexCoords;\n"

    "void main() {\n"
    "v_TexCoords = a_TexCoords;\n"
    "gl_Position = vec4(a_Position, 1.0, 1.0);\n"
    "}\n";

const char *const kGm_FragmentShaderSource =
    "#version 330 core\n"

    "in vec2 v_TexCoords;\n"

    "out vec4 f_Color;\n"

    "vec2 ComplexMul(vec2 a, vec2 b) {\n"
    "return vec2(a.x * b.x - a.y * b.y, a.x * b.y + a.y * b.x);\n"
    "}\n"

    "vec2 ComplexSq(vec2 z) {\n"
    "return ComplexMul(z, z);\n"
    "}\n"

    "float ComplexSqMag(vec2 z) {\n"
    "return z.x * z.x + z.y * z.y;\n"
    "}\n"

    "vec3 HsvToRgb(vec3 hsv) {\n"
    "vec4 K = vec4(1.0, 2.0 / 3.0, 1.0 / 3.0, 3.0);\n"
    "vec3 p = abs(fract(hsv.xxx + K.xyz) * 6.0 - K.www);\n"
    "return hsv.z * mix(K.xxx, clamp(p - K.xxx, 0.0, 1.0), hsv.y);\n"
    "}\n"

    "vec3 IterToRgb(int iterations) {\n"
    "int h_deg = iterations % 360;\n"
    "vec3 hsv = vec3(float(h_deg) / 360.0, 0.9, 1.0);\n"
    "return HsvToRgb(hsv);\n"
    "}\n"

    "const int kMaxIterations = 100;\n"

    "void main() {\n"
    "vec2 c = v_TexCoords * 2.0 - vec2(1.5, 1.0);\n"
    "vec2 z = c;\n"

    "int i = 0;\n"
    "for (; (i < kMaxIterations) && (ComplexSqMag(z) < 16.0); ++i) {\n"
    "z = ComplexSq(z) + c;"
    "}\n"

    "if (i == kMaxIterations) {\n"
    "f_Color = vec4(0.0);\n"
    "} else {\n"
    "f_Color = vec4(IterToRgb(i), 1.0);\n"
    "}\n"
    "}\n";
// clang-format on

gm_Status gm_CheckShaderCompilationStatus(GLuint shader);

gm_Status gm_CreateShaders(gm_ProgramShaders *out_shaders) {
  out_shaders->vertex = glCreateShader(GL_VERTEX_SHADER);
  out_shaders->fragment = glCreateShader(GL_FRAGMENT_SHADER);
  GM_PRINT_GL_ERROR_STATUS("create shaders");

  glShaderSource(out_shaders->vertex, 1, &kGm_VertexShaderSource, NULL);
  glShaderSource(out_shaders->fragment, 1, &kGm_FragmentShaderSource, NULL);
  GM_PRINT_GL_ERROR_STATUS("upload shader sources");

  glCompileShader(out_shaders->vertex);
  glCompileShader(out_shaders->fragment);
  GM_PRINT_GL_ERROR_STATUS("compile shaders");

  if (gm_CheckShaderCompilationStatus(out_shaders->vertex) ||
      gm_CheckShaderCompilationStatus(out_shaders->fragment)) {
    gm_DestroyShaders(out_shaders);
    return gm_Status_ShaderCompilationFailed;
  }

  return gm_Status_Success;
}

gm_Status gm_CheckShaderCompilationStatus(GLuint shader) {
  int compile_status;
  glGetShaderiv(shader, GL_COMPILE_STATUS, &compile_status);

  if (!compile_status) {
    int length;
    glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &length);

    GLchar *info_log = malloc(length);
    glGetShaderInfoLog(shader, length, NULL, info_log);

    fprintf(stderr, "Shader compilation failed:\n%s", info_log);
    return gm_Status_ShaderCompilationFailed;
  }

  return gm_Status_Success;
}

void gm_DestroyShaders(const gm_ProgramShaders *shaders) {
  glDeleteShader(shaders->vertex);
  glDeleteShader(shaders->fragment);
}

void gm_Terminate() {
  glfwDestroyWindow(window);
  glfwMakeContextCurrent(NULL);
  glfwTerminate();
}

const char *gm_GetStatusCodeMessage(int status_code);

void gm_PrintStatus(FILE *stream, int status_code) {
  const char *const kStatusInfo = gm_GetStatusCodeMessage(status_code);
  fprintf(stream, "%s\n", kStatusInfo);
}

const char *gm_GetStatusCodeMessage(int status_code) {
  switch (status_code) {
    case gm_Status_Success:
      return "Everything went as planned";
    case gm_Status_GlfwInitFailed:
      return "GLFW initialization failed";
    case gm_Status_DummyWindowCreationFailed:
      return "Creation of the dummy window failed";
    case gm_Status_GlLoadingFailed:
      return "Failed to load OpenGL functions";
    case gm_Status_ShaderCompilationFailed:
      return "Failed to compile a shader";
    case gm_Status_FramebufferCreationFailed:
      return "Failed to create the image framebuffer";
    case gm_Status_ImageOutputFailed:
      return "Failed to write to the output image";
    default:
      return "Unknown status";
  }
}
