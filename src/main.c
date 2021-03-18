#define GLFW_INCLUDE_NONE
#define STB_IMAGE_WRITE_IMPLEMENTATION

#include <GLFW/glfw3.h>
#include <glad/glad.h>
#include <stb/stb_image_write.h>
#include <stdio.h>
#include <stdlib.h>

typedef enum {
  Status_Success,
  Status_GlfwInitFailed,
  Status_DummyWindowCreationFailed,
  Status_GlLoadingFailed,
  Status_ShaderCompilationFailed,
  Status_ProgramLinkingFailed,
  Status_FramebufferCreationFailed,
  Status_ImageOutputFailed
} Status;

Status WriteMandelbrotToOutput();
void PrintStatus(FILE *stream, int status_code);

int main() {
  const int kStatus = WriteMandelbrotToOutput();
  if (kStatus) {
    PrintStatus(stderr, kStatus);
  } else {
    PrintStatus(stdout, kStatus);
  }
  return kStatus;
}

// clang-format off
const char *const kVertexShaderSource =
    "#version 330 core \n"

    "layout(location = 0) in vec2 a_Position;\n"
    "layout(location = 1) in vec2 a_TexCoords;\n"

    "out vec2 v_TexCoords;\n"

    "void main() {\n"
      "v_TexCoords = a_TexCoords;\n"
      "gl_Position = vec4(a_Position, 1.0, 1.0);\n"
    "}\n";

const char *const kFragmentShaderSource =
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

const int kImageWidth = 500;
const int kImageHeight = 500;

#ifndef NDEBUG
#  define PRINT_GL_ERROR_STATUS(info) \
    fprintf(stdout, "Error status: %d (%s)\n", glGetError(), info)
#else
#  define PRINT_GL_ERROR_STATUS(...)
#endif

Status Init();

Status CheckShaderCompilationStatus(GLuint shader);
Status CheckProgramLinkStatus(GLuint program);

void Terminate();

Status WriteMandelbrotToOutput() {
  const Status kInitStatus = Init();
  if (kInitStatus) {
    return kInitStatus;
  }

  // Create the shaders.
  GLuint vertex_shader = glCreateShader(GL_VERTEX_SHADER);
  GLuint fragment_shader = glCreateShader(GL_FRAGMENT_SHADER);
  PRINT_GL_ERROR_STATUS("create shaders");

  glShaderSource(vertex_shader, 1, &kVertexShaderSource, NULL);
  glShaderSource(fragment_shader, 1, &kFragmentShaderSource, NULL);
  PRINT_GL_ERROR_STATUS("shader sources");

  glCompileShader(vertex_shader);
  glCompileShader(fragment_shader);
  PRINT_GL_ERROR_STATUS("compile shaders");

  if (CheckShaderCompilationStatus(vertex_shader) ||
      CheckShaderCompilationStatus(fragment_shader)) {
    return Status_ShaderCompilationFailed;
  }

  // Create the program.
  GLuint program = glCreateProgram();
  PRINT_GL_ERROR_STATUS("create program");

  glAttachShader(program, vertex_shader);
  glAttachShader(program, fragment_shader);
  PRINT_GL_ERROR_STATUS("attach shaders");

  glLinkProgram(program);
  PRINT_GL_ERROR_STATUS("link program");

  if (CheckProgramLinkStatus(program)) {
    return Status_ProgramLinkingFailed;
  }

  // Now that the program is linked, delete the shaders.
  glDeleteShader(vertex_shader);
  glDeleteShader(fragment_shader);
  PRINT_GL_ERROR_STATUS("delete shaders");

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
  PRINT_GL_ERROR_STATUS("gen buffers");

  // Send the data to the buffers.
  glBindBuffer(GL_ARRAY_BUFFER, buffers[0]);
  glBufferData(GL_ARRAY_BUFFER, sizeof(kVertices), kVertices, GL_STATIC_DRAW);
  PRINT_GL_ERROR_STATUS("vertex data");

  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, buffers[1]);
  glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(kIndices), kIndices,
               GL_STATIC_DRAW);
  PRINT_GL_ERROR_STATUS("index data");

  // Vertex layout.
  const GLsizei kStride = 4 * sizeof(GLfloat);

  glEnableVertexAttribArray(0);
  glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, kStride, NULL);

  const void *kTexCoordsOffset = (void *)(2 * sizeof(GLfloat));
  glEnableVertexAttribArray(1);
  glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, kStride, kTexCoordsOffset);

  PRINT_GL_ERROR_STATUS("vertex attrib");

  glBindVertexArray(0);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
  glBindBuffer(GL_ARRAY_BUFFER, 0);

  // Create and initialize the framebuffer renderbuffer.
  GLuint image_buf;
  glGenRenderbuffers(1, &image_buf);
  glBindRenderbuffer(GL_RENDERBUFFER, image_buf);
  PRINT_GL_ERROR_STATUS("create image buf");

  glRenderbufferStorage(GL_RENDERBUFFER, GL_RGB8, kImageWidth, kImageHeight);
  PRINT_GL_ERROR_STATUS("image buf storage");

  glBindRenderbuffer(GL_RENDERBUFFER, 0);

  // Create the framebuffer and use it.
  GLuint fb;
  glGenFramebuffers(1, &fb);
  glBindFramebuffer(GL_FRAMEBUFFER, fb);
  PRINT_GL_ERROR_STATUS("create fb");

  // Attach it to the framebuffer.
  glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0,
                            GL_RENDERBUFFER, image_buf);
  PRINT_GL_ERROR_STATUS("attach image buf to fb");

  if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE) {
    return Status_FramebufferCreationFailed;
  }

  glBindFramebuffer(GL_FRAMEBUFFER, 0);

  // Not doing this results in the image missing some parts.
  glViewport(0, 0, kImageWidth, kImageHeight);
  PRINT_GL_ERROR_STATUS("set viewport");

  // Render a quad on the framebuffer.
  glBindFramebuffer(GL_FRAMEBUFFER, fb);
  glUseProgram(program);
  glBindVertexArray(quad_va);

  // TODO: Render to a multi-sampled framebuffer and blit to a regular
  // TODO: framebuffer before reading.
  glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_SHORT, NULL);
  PRINT_GL_ERROR_STATUS("draw elements");

  // Retrieve the image data from the framebuffer.
  unsigned char *data = malloc(kImageWidth * kImageHeight * 3);  // 3 for RGB.

  glBindFramebuffer(GL_FRAMEBUFFER, 0);
  glBindFramebuffer(GL_READ_FRAMEBUFFER, fb);
  PRINT_GL_ERROR_STATUS("bind fb to read framebuffer");

  glReadBuffer(GL_COLOR_ATTACHMENT0);
  PRINT_GL_ERROR_STATUS("set read buffer");

  glReadPixels(0, 0, kImageWidth, kImageHeight, GL_RGB, GL_UNSIGNED_BYTE, data);
  PRINT_GL_ERROR_STATUS("read pixels");

  // Write the image data to a file.
  const int kResult = stbi_write_png("output.png", kImageWidth, kImageHeight, 3,
                                     data, 3 * kImageWidth);

  if (!kResult) {
    fputs("Failed to write image\n", stderr);
    return Status_ImageOutputFailed;
  }

  // Cleanup data.
  glDeleteRenderbuffers(1, &image_buf);
  glDeleteFramebuffers(1, &fb);
  glDeleteProgram(program);
  glDeleteBuffers(2, buffers);
  PRINT_GL_ERROR_STATUS("delete objects");

  Terminate();
  return Status_Success;
}

/**
 * Dummy window used to create an OpenGL context.
 */
GLFWwindow *window;

Status Init() {
  if (!glfwInit()) {
    return Status_GlfwInitFailed;
  }

  glfwDefaultWindowHints();
  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
  glfwWindowHint(GLFW_VISIBLE, GLFW_FALSE);

  window = glfwCreateWindow(1, 1, "", NULL, NULL);
  if (!window) {
    return Status_DummyWindowCreationFailed;
  }

  glfwMakeContextCurrent(window);
  if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
    return Status_GlLoadingFailed;
  }

  return Status_Success;
}

const char *GetStatusInfo(int status_code);

void PrintStatus(FILE *stream, int status_code) {
  const char *const kStatusInfo = GetStatusInfo(status_code);
  fprintf(stream, "%s\n", kStatusInfo);
}

const char *GetStatusInfo(int status_code) {
  switch (status_code) {
    case Status_Success:
      return "Everything went as planned";
    case Status_GlfwInitFailed:
      return "GLFW initialization failed";
    case Status_DummyWindowCreationFailed:
      return "Creation of the dummy window failed";
    case Status_GlLoadingFailed:
      return "Failed to load OpenGL functions";
    case Status_ShaderCompilationFailed:
      return "Failed to compile a shader";
    case Status_FramebufferCreationFailed:
      return "Failed to create the image framebuffer";
    case Status_ImageOutputFailed:
      return "Failed to write to the output image";
    default:
      return "Unknown status";
  }
}

Status CheckShaderCompilationStatus(GLuint shader) {
  int compile_status;
  glGetShaderiv(shader, GL_COMPILE_STATUS, &compile_status);

  if (!compile_status) {
    int length;
    glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &length);

    GLchar *info_log = malloc(length);
    glGetShaderInfoLog(shader, length, NULL, info_log);

    fprintf(stderr, "Shader compilation failed:\n%s", info_log);
    return Status_ShaderCompilationFailed;
  }

  return Status_Success;
}

Status CheckProgramLinkStatus(GLuint program) {
  int link_status;
  glGetProgramiv(program, GL_LINK_STATUS, &link_status);

  if (!link_status) {
    int length;
    glGetProgramiv(program, GL_INFO_LOG_LENGTH, &length);

    GLchar *info_log = malloc(length);
    glGetProgramInfoLog(program, length, NULL, info_log);

    fprintf(stderr, "Program linking failed:\n%s", info_log);
    return Status_ProgramLinkingFailed;
  }

  return Status_Success;
}

void Terminate() {
  glfwDestroyWindow(window);
  glfwTerminate();
}
